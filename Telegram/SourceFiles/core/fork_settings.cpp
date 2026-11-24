/*
This file is part of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/telegramdesktop/tdesktop/blob/master/LEGAL
*/
#include "core/fork_settings.h"

#include "core/core_settings.h"

namespace Core {

ForkSettings::ForkSettings(Settings *owner) : _owner(owner) {
	reload();
}

void ForkSettings::reload() {
	_squareUserpics = _owner->readPref<bool>(kSquareUserpics, false);
	_audioFade = _owner->readPref<bool>(kAudioFade, true);
	_autoSubmitPasscode = _owner->readPref<bool>(kAutoSubmitPasscode, false);
	_copyLoginCode = _owner->readPref<bool>(kCopyLoginCode, false);
	_widgetsDisabled = _owner->readPref<bool>(kWidgetsDisabled, false);
	_lastSeenInDialogs = _owner->readPref<bool>(kLastSeenInDialogs, false);
	_askUriScheme = _owner->readPref<bool>(kAskUriScheme, false);
	_uriScheme = QString::fromUtf8(
		_owner->readPref<QByteArray>(kUriScheme));
	_searchSelected = _owner->readPref<bool>(kSearchSelected, false);
	_searchEngineUrl = QString::fromUtf8(
		_owner->readPref<QByteArray>(
			kSearchEngineUrl,
			u"https://duckduckgo.com/?q=%q"_q.toUtf8()));
	_customStickerSize = _owner->readPref<int>(kCustomStickerSize, 256);
	_useBlackTrayIcon = _owner->readPref<bool>(kUseBlackTrayIcon, false);
	_useOriginalTrayIcon = _owner->readPref<bool>(kUseOriginalTrayIcon, false);
	_emojiPopupOnClick = _owner->readPref<bool>(kEmojiPopupOnClick, false);
	_addToMenuRememberMedia = _owner->readPref<bool>(
		kAddToMenuRememberMedia,
		false);
	_hideAllChatsTab = _owner->readPref<bool>(kHideAllChatsTab, false);
	_globalSearchDisabled = _owner->readPref<bool>(kGlobalSearchDisabled, false);
	_skipShareFromBot = _owner->readPref<bool>(kSkipShareFromBot, false);
	_additionalButtonsWebBot = _owner->readPref<bool>(
		kAdditionalButtonsWebBot,
		false);
	_botsPlatforms = QString::fromUtf8(
		_owner->readPref<QByteArray>(kBotsPlatforms));
	_archivedStoriesAreHidden = _owner->readPref<bool>(
		kArchivedStoriesAreHidden,
		false);
	_hideFromBlockedUsers = _owner->readPref<bool>(kHideFromBlockedUsers, false);
	_markdownClipboardText = QString::fromUtf8(
		_owner->readPref<QByteArray>(kMarkdownClipboardText));
	_emojiSearchLanguages = QString::fromUtf8(
		_owner->readPref<QByteArray>(kEmojiSearchLanguages));
}

void ForkSettings::resetOnLastLogout() {
	for (const auto key : kAllKeys) {
		_owner->clearPref(key);
	}
	reload();
	_changes.fire({});
}

void ForkSettings::assign(bool &field, bool value, std::string_view key) {
	if (field == value) {
		return;
	}
	field = value;
	_owner->writePref<bool>(key, value);
	_changes.fire({});
}

void ForkSettings::assign(int &field, int value, std::string_view key) {
	if (field == value) {
		return;
	}
	field = value;
	_owner->writePref<int>(key, value);
	_changes.fire({});
}

void ForkSettings::assign(
		QString &field,
		const QString &value,
		std::string_view key) {
	if (field == value) {
		return;
	}
	field = value;
	_owner->writePref<QByteArray>(key, value.toUtf8());
	_changes.fire({});
}

rpl::producer<> ForkSettings::changes() const {
	return _changes.events();
}

} // namespace Core
