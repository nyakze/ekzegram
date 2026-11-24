/*
This file is part of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/telegramdesktop/tdesktop/blob/master/LEGAL

The send-as-copy API is adapted from Forkgram (https://github.com/forkgram/tdesktop).
*/
#pragma once

#include "ui/text/text_entity.h"

#include <vector>

class HistoryItem;
class PeerData;

namespace Api::AsCopy {

struct ToSend {
	std::vector<not_null<PeerData*>> peers;
	TextWithTags comment;
	bool emptyText = false;
	bool silent = false;
	TimeId scheduled = 0;
};

[[nodiscard]] bool CanResend(not_null<HistoryItem*> item);

void GuardedSendExistingAlbumFromItem(
	not_null<HistoryItem*> item,
	ToSend &&toSend);

void SendExistingMediaFromItem(not_null<HistoryItem*> item, ToSend &&toSend);

void SendAlbumFromItems(
	HistoryItemsList items,
	ToSend &&toSend,
	bool andDelete);

void UpdateFileRef(
	HistoryItemsList list,
	Fn<void()> success,
	Fn<void(QString)> fail);

} // namespace Api::AsCopy
