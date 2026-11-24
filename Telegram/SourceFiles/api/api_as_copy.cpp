/*
This file is part of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/telegramdesktop/tdesktop/blob/master/LEGAL

The send-as-copy API is adapted from Forkgram (https://github.com/forkgram/tdesktop).
*/
#include "api/api_as_copy.h"

#include "api/api_common.h"
#include "api/api_sending.h"
#include "api/api_text_entities.h"
#include "apiwrap.h"
#include "base/algorithm.h"
#include "base/random.h"
#include "chat_helpers/message_field.h"
#include "data/data_channel.h"
#include "data/data_document.h"
#include "data/data_drafts.h"
#include "data/data_histories.h"
#include "data/data_photo.h"
#include "data/data_session.h"
#include "history/history.h"
#include "history/history_item.h"
#include "main/main_session.h"

namespace Api::AsCopy {
namespace {

MTPInputSingleMedia PrepareAlbumItemMedia(
		not_null<HistoryItem*> item,
		const MTPInputMedia &media,
		uint64 randomId,
		bool emptyText,
		TextWithTags comment) {
	auto commentEntities = TextWithEntities{
		comment.text,
		TextUtilities::ConvertTextTagsToEntities(comment.tags)
	};

	auto caption = item->originalText();
	TextUtilities::Trim(caption);
	auto sentEntities = Api::EntitiesToMTP(
		&item->history()->session(),
		emptyText ? commentEntities.entities : caption.entities,
		Api::ConvertOption::SkipLocal);
	const auto flags = !sentEntities.v.isEmpty()
		? MTPDinputSingleMedia::Flag::f_entities
		: MTPDinputSingleMedia::Flag(0);

	return MTP_inputSingleMedia(
		MTP_flags(flags),
		media,
		MTP_long(randomId),
		MTP_string(emptyText ? commentEntities.text : caption.text),
		sentEntities);
}

MTPinputMedia InputMediaFromItem(not_null<HistoryItem*> i) {
	const auto media = i->media();
	if (!media) {
		return MTP_inputMediaEmpty();
	} else if (const auto document = media->document()) {
		return MTP_inputMediaDocument(
			MTP_flags(MTPDinputMediaDocument::Flag(0)),
			document->mtpInput(),
			document->goodThumbnailPhoto()
				? document->goodThumbnailPhoto()->mtpInput()
				: MTPInputPhoto(), // video_cover
			MTPint(), // video_timestamp
			MTPint(), // ttl_seconds
			MTPstring()); // query
	} else if (const auto photo = media->photo()) {
		return MTP_inputMediaPhoto(
			MTP_flags(MTPDinputMediaPhoto::Flag(0)),
			photo->mtpInput(),
			MTPint(), // ttl_seconds
			MTPInputDocument()); // video
	}
	return MTP_inputMediaEmpty();
}

FullReplyTo ReplyToIdFromDraft(not_null<PeerData*> peer) {
	const auto history = peer->owner().history(peer);
	const auto replyTo = [&]() -> FullReplyTo {
		if (const auto localDraft = history->localDraft(0, 0)) {
			return localDraft->reply;
		} else if (const auto cloudDraft = history->cloudDraft(0, 0)) {
			return cloudDraft->reply;
		} else {
			return {};
		}
	}();
	if (replyTo) {
		history->clearCloudDraft(0, 0);
		history->clearLocalDraft(0, 0);
		peer->session().api().request(
			MTPmessages_SaveDraft(
				MTP_flags(MTPmessages_SaveDraft::Flags(0)),
				MTPInputReplyTo(),
				history->peer->input(),
				MTPstring(),
				MTPVector<MTPMessageEntity>(),
				MTP_inputMediaEmpty(),
				MTP_long(0), // effect
				SuggestToMTP({}),
				MTPInputRichMessage()
		)).send();
	}
	return replyTo;
}

} // namespace

bool CanResend(not_null<HistoryItem*> item) {
	return item->allowsForward() && !item->forbidsSaving();
}

void SendAlbumFromItems(
		HistoryItemsList items,
		ToSend &&toSend,
		bool andDelete) {
	auto allowed = HistoryItemsList();
	for (const auto &item : items) {
		if (CanResend(item)) {
			allowed.push_back(item);
		}
	}
	if (allowed.empty()) {
		return;
	}
	const auto history = allowed.front()->history();
	const auto ids = history->owner().itemsToIds(allowed);
	auto medias = QVector<MTPInputSingleMedia>();
	for (const auto &i : allowed) {
		medias.push_back(PrepareAlbumItemMedia(
			i,
			InputMediaFromItem(i),
			base::RandomValue<uint64>(),
			toSend.emptyText,
			medias.empty() ? toSend.comment : TextWithTags()));
	}
	auto &api = history->owner().session().api();

	for (const auto &peer : toSend.peers) {
		const auto target = peer->owner().history(peer);
		const auto replyTo = ReplyToIdFromDraft(peer);

		const auto flags = MTPmessages_SendMultiMedia::Flags(0)
			| (replyTo
				? MTPmessages_SendMultiMedia::Flag::f_reply_to
				: MTPmessages_SendMultiMedia::Flag(0))
			| (toSend.silent
				? MTPmessages_SendMultiMedia::Flag::f_silent
				: MTPmessages_SendMultiMedia::Flag(0))
			| (toSend.scheduled
				? MTPmessages_SendMultiMedia::Flag::f_schedule_date
				: MTPmessages_SendMultiMedia::Flag(0));
		api.request(MTPmessages_SendMultiMedia(
			MTP_flags(flags),
			peer->input(),
			ReplyToForMTP(target, replyTo),
			MTP_vector<MTPInputSingleMedia>(medias),
			MTP_int(toSend.scheduled),
			MTP_inputPeerEmpty(),
			MTPInputQuickReplyShortcut(),
			MTP_long(0),
			MTP_long(0)
		)).done([=](const MTPUpdates &result) {
			history->owner().session().api().applyUpdates(result);

			if (andDelete) {
				history->owner().histories().deleteMessages(ids, true);
				history->owner().sendHistoryChangeNotifications();
			}
		}).send();
	}
}

void GuardedSendExistingAlbumFromItem(
		not_null<HistoryItem*> item,
		ToSend &&toSend) {
	if (!item->groupId() || !CanResend(item)) {
		return;
	}
	const auto group = item->history()->owner().groups().find(item);
	if (!group) {
		return;
	}
	const auto items = group->items;
	UpdateFileRef(
		items,
		[=] { SendAlbumFromItems(items, base::duplicate(toSend), false); },
		[](QString) {});
}

void SendExistingMediaFromItem(
		not_null<HistoryItem*> item,
		ToSend &&toSend) {
	if (!CanResend(item)) {
		return;
	}
	for (const auto &peer : toSend.peers) {
		const auto history = peer->owner().history(peer);
		auto message = MessageToSend(SendAction{ history });
		if (!item->media()) {
			message.textWithTags = PrepareEditText(item);
			history->session().api().sendMessage(std::move(message));
			continue;
		}
		message.textWithTags = toSend.emptyText
			? toSend.comment
			: PrepareEditText(item);
		message.action.options.silent = toSend.silent;
		message.action.options.scheduled = toSend.scheduled;
		message.action.replyTo = ReplyToIdFromDraft(peer);
		if (const auto document = item->media()->document()) {
			Api::SendExistingDocument(std::move(message), document);
		} else if (const auto photo = item->media()->photo()) {
			Api::SendExistingPhoto(std::move(message), photo);
		}
	}
}

void UpdateFileRef(
		HistoryItemsList list,
		Fn<void()> success,
		Fn<void(QString)> fail) {
	if (list.empty()) {
		return;
	}
	const auto history = list.front()->history();
	auto inputMessages = ranges::views::all(
		list
	) | ranges::views::transform([&](const auto &item) {
		return MTP_inputMessageID(MTP_int(item->id));
	}) | ranges::to<QVector<MTPInputMessage>>();
	const auto receive = [=](const MTPmessages_Messages &result) {
		result.match([&](const MTPDmessages_messagesNotModified &) {
			fail(u"MTPDmessages_messagesNotModified"_q);
		}, [&](const auto &d) {
			auto good = true;
			for (const auto &tlMessage : d.vmessages().v) {
				tlMessage.match([&](const MTPDmessage &d) {
					history->session().data().updateExistingMessage(d);
				}, [&](const auto &) {
					good = false;
					fail(u"MTPDmessageService or MTPDmessageEmpty"_q);
				});
			}
			if (good) {
				success();
			}
		});
	};
	const auto receiveError = [=](const MTP::Error &error) {
		fail(u"Get Message error: "_q + error.type());
	};
	if (const auto channel = history->peer->asChannel()) {
		history->session().api().request(
			MTPchannels_GetMessages(
				channel->inputChannel(),
				MTP_vector<MTPInputMessage>(std::move(inputMessages)))
		).done(receive).fail(receiveError).send();
	} else {
		history->session().api().request(
			MTPmessages_GetMessages(
				MTP_vector<MTPInputMessage>(std::move(inputMessages)))
		).done(receive).fail(receiveError).send();
	}
}

} // namespace Api::AsCopy
