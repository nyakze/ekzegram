/*
This file is part of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/telegramdesktop/tdesktop/blob/master/LEGAL

The settings list is adapted from Forkgram (https://github.com/forkgram/tdesktop).
*/
#pragma once

#include "base/basic_types.h"

#include <array>
#include <string_view>

#include <rpl/rpl.h>

#include <QtCore/QByteArray>
#include <QtCore/QString>

namespace Core {

class Settings;

class ForkSettings final {
public:
	explicit ForkSettings(Settings *owner);

	void reload();
	void resetOnLastLogout();

	[[nodiscard]] bool squareUserpics() const {
		return _squareUserpics;
	}
	void setSquareUserpics(bool value) {
		assign(_squareUserpics, value, kSquareUserpics);
	}

	[[nodiscard]] bool audioFade() const {
		return _audioFade;
	}
	void setAudioFade(bool value) {
		assign(_audioFade, value, kAudioFade);
	}

	[[nodiscard]] bool autoSubmitPasscode() const {
		return _autoSubmitPasscode;
	}
	void setAutoSubmitPasscode(bool value) {
		assign(_autoSubmitPasscode, value, kAutoSubmitPasscode);
	}

	[[nodiscard]] bool copyLoginCode() const {
		return _copyLoginCode;
	}
	void setCopyLoginCode(bool value) {
		assign(_copyLoginCode, value, kCopyLoginCode);
	}

	[[nodiscard]] bool widgetsDisabled() const {
		return _widgetsDisabled;
	}
	void setWidgetsDisabled(bool value) {
		assign(_widgetsDisabled, value, kWidgetsDisabled);
	}

	[[nodiscard]] bool lastSeenInDialogs() const {
		return _lastSeenInDialogs;
	}
	void setLastSeenInDialogs(bool value) {
		assign(_lastSeenInDialogs, value, kLastSeenInDialogs);
	}

	[[nodiscard]] bool askUriScheme() const {
		return _askUriScheme;
	}
	void setAskUriScheme(bool value) {
		assign(_askUriScheme, value, kAskUriScheme);
	}

	[[nodiscard]] QString uriScheme() const {
		return _uriScheme;
	}
	void setUriScheme(const QString &value) {
		assign(_uriScheme, value, kUriScheme);
	}

	[[nodiscard]] bool searchSelected() const {
		return _searchSelected;
	}
	void setSearchSelected(bool value) {
		assign(_searchSelected, value, kSearchSelected);
	}

	[[nodiscard]] QString searchEngineUrl() const {
		return _searchEngineUrl;
	}
	void setSearchEngineUrl(const QString &value) {
		assign(_searchEngineUrl, value, kSearchEngineUrl);
	}

	[[nodiscard]] int customStickerSize() const {
		return _customStickerSize;
	}
	void setCustomStickerSize(int value) {
		assign(_customStickerSize, value, kCustomStickerSize);
	}

	[[nodiscard]] bool useBlackTrayIcon() const {
		return _useBlackTrayIcon;
	}
	void setUseBlackTrayIcon(bool value) {
		assign(_useBlackTrayIcon, value, kUseBlackTrayIcon);
	}

	[[nodiscard]] bool useOriginalTrayIcon() const {
		return _useOriginalTrayIcon;
	}
	void setUseOriginalTrayIcon(bool value) {
		assign(_useOriginalTrayIcon, value, kUseOriginalTrayIcon);
	}

	[[nodiscard]] bool emojiPopupOnClick() const {
		return _emojiPopupOnClick;
	}
	void setEmojiPopupOnClick(bool value) {
		assign(_emojiPopupOnClick, value, kEmojiPopupOnClick);
	}

	[[nodiscard]] bool addToMenuRememberMedia() const {
		return _addToMenuRememberMedia;
	}
	void setAddToMenuRememberMedia(bool value) {
		assign(_addToMenuRememberMedia, value, kAddToMenuRememberMedia);
	}

	[[nodiscard]] bool hideAllChatsTab() const {
		return _hideAllChatsTab;
	}
	void setHideAllChatsTab(bool value) {
		assign(_hideAllChatsTab, value, kHideAllChatsTab);
	}

	[[nodiscard]] bool globalSearchDisabled() const {
		return _globalSearchDisabled;
	}
	void setGlobalSearchDisabled(bool value) {
		assign(_globalSearchDisabled, value, kGlobalSearchDisabled);
	}

	[[nodiscard]] bool skipShareFromBot() const {
		return _skipShareFromBot;
	}
	void setSkipShareFromBot(bool value) {
		assign(_skipShareFromBot, value, kSkipShareFromBot);
	}

	[[nodiscard]] bool additionalButtonsWebBot() const {
		return _additionalButtonsWebBot;
	}
	void setAdditionalButtonsWebBot(bool value) {
		assign(_additionalButtonsWebBot, value, kAdditionalButtonsWebBot);
	}

	[[nodiscard]] QString botsPlatforms() const {
		return _botsPlatforms;
	}
	void setBotsPlatforms(const QString &value) {
		assign(_botsPlatforms, value, kBotsPlatforms);
	}

	[[nodiscard]] bool archivedStoriesAreHidden() const {
		return _archivedStoriesAreHidden;
	}
	void setArchivedStoriesAreHidden(bool value) {
		assign(_archivedStoriesAreHidden, value, kArchivedStoriesAreHidden);
	}

	[[nodiscard]] bool hideFromBlockedUsers() const {
		return _hideFromBlockedUsers;
	}
	void setHideFromBlockedUsers(bool value) {
		assign(_hideFromBlockedUsers, value, kHideFromBlockedUsers);
	}

	[[nodiscard]] QString markdownClipboardText() const {
		return _markdownClipboardText;
	}
	void setMarkdownClipboardText(const QString &value) {
		assign(_markdownClipboardText, value, kMarkdownClipboardText);
	}

	[[nodiscard]] QString emojiSearchLanguages() const {
		return _emojiSearchLanguages;
	}
	void setEmojiSearchLanguages(const QString &value) {
		assign(_emojiSearchLanguages, value, kEmojiSearchLanguages);
	}

	[[nodiscard]] bool forwardedDateTitle() const {
		return _forwardedDateTitle;
	}
	void setForwardedDateTitle(bool value) {
		assign(_forwardedDateTitle, value, kForwardedDateTitle);
	}

	[[nodiscard]] rpl::producer<> changes() const;

private:
	static constexpr auto kSquareUserpics = "fork.squareUserpics";
	static constexpr auto kAudioFade = "fork.audioFade";
	static constexpr auto kAutoSubmitPasscode = "fork.autoSubmitPasscode";
	static constexpr auto kCopyLoginCode = "fork.copyLoginCode";
	static constexpr auto kWidgetsDisabled = "fork.widgetsDisabled";
	static constexpr auto kLastSeenInDialogs = "fork.lastSeenInDialogs";
	static constexpr auto kAskUriScheme = "fork.askUriScheme";
	static constexpr auto kUriScheme = "fork.uriScheme";
	static constexpr auto kSearchSelected = "fork.searchSelected";
	static constexpr auto kSearchEngineUrl = "fork.searchEngineUrl";
	static constexpr auto kCustomStickerSize = "fork.customStickerSize";
	static constexpr auto kUseBlackTrayIcon = "fork.useBlackTrayIcon";
	static constexpr auto kUseOriginalTrayIcon = "fork.useOriginalTrayIcon";
	static constexpr auto kEmojiPopupOnClick = "fork.emojiPopupOnClick";
	static constexpr auto kAddToMenuRememberMedia = "fork.addToMenuRememberMedia";
	static constexpr auto kHideAllChatsTab = "fork.hideAllChatsTab";
	static constexpr auto kGlobalSearchDisabled = "fork.globalSearchDisabled";
	static constexpr auto kSkipShareFromBot = "fork.skipShareFromBot";
	static constexpr auto kAdditionalButtonsWebBot = "fork.additionalButtonsWebBot";
	static constexpr auto kBotsPlatforms = "fork.botsPlatforms";
	static constexpr auto kArchivedStoriesAreHidden = "fork.archivedStoriesAreHidden";
	static constexpr auto kHideFromBlockedUsers = "fork.hideFromBlockedUsers";
	static constexpr auto kMarkdownClipboardText = "fork.markdownClipboardText";
	static constexpr auto kEmojiSearchLanguages = "fork.emojiSearchLanguages";
	static constexpr auto kForwardedDateTitle = "fork.forwardedDateTitle";

	static constexpr auto kAllKeys = std::array<std::string_view, 25>{
		kSquareUserpics,
		kAudioFade,
		kAutoSubmitPasscode,
		kCopyLoginCode,
		kWidgetsDisabled,
		kLastSeenInDialogs,
		kAskUriScheme,
		kUriScheme,
		kSearchSelected,
		kSearchEngineUrl,
		kCustomStickerSize,
		kUseBlackTrayIcon,
		kUseOriginalTrayIcon,
		kEmojiPopupOnClick,
		kAddToMenuRememberMedia,
		kHideAllChatsTab,
		kGlobalSearchDisabled,
		kSkipShareFromBot,
		kAdditionalButtonsWebBot,
		kBotsPlatforms,
		kArchivedStoriesAreHidden,
		kHideFromBlockedUsers,
		kMarkdownClipboardText,
		kEmojiSearchLanguages,
		kForwardedDateTitle,
	};

	void assign(bool &field, bool value, std::string_view key);
	void assign(int &field, int value, std::string_view key);
	void assign(QString &field, const QString &value, std::string_view key);

	Settings *_owner = nullptr;

	bool _squareUserpics = false;
	bool _audioFade = true;
	bool _autoSubmitPasscode = false;
	bool _copyLoginCode = false;
	bool _widgetsDisabled = false;
	bool _lastSeenInDialogs = false;
	bool _askUriScheme = false;
	QString _uriScheme;
	bool _searchSelected = false;
	QString _searchEngineUrl = u"https://duckduckgo.com/?q=%q"_q;
	int _customStickerSize = 256;
	bool _useBlackTrayIcon = false;
	bool _useOriginalTrayIcon = false;
	bool _emojiPopupOnClick = false;
	bool _addToMenuRememberMedia = false;
	bool _hideAllChatsTab = false;
	bool _globalSearchDisabled = false;
	bool _skipShareFromBot = false;
	bool _additionalButtonsWebBot = false;
	QString _botsPlatforms;
	bool _archivedStoriesAreHidden = false;
	bool _hideFromBlockedUsers = false;
	QString _markdownClipboardText;
	QString _emojiSearchLanguages;
	bool _forwardedDateTitle = false;

	rpl::event_stream<> _changes;

};

} // namespace Core
