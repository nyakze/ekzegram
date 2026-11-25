/*
This file is part of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/telegramdesktop/tdesktop/blob/master/LEGAL
*/
#include "settings/sections/settings_fork.h"

#include "settings/settings_common_session.h"
#include "chat_helpers/emoji_keywords.h"
#include "core/application.h"
#include "core/core_settings.h"
#include "forkgram/link_replacements.h"
#include "forkgram/link_replacements_box.h"
#include "forkgram/uri_menu.h"
#include "history/view/history_view_context_menu_fork.h"
#include "lang/lang_keys.h"
#include "menu/menu_item_save_to_markdown.h"
#include "settings/sections/settings_main.h"
#include "settings/settings_builder.h"
#include "spellcheck/spellcheck_types.h"
#include "ui/boxes/choose_language_box.h"
#include "ui/boxes/confirm_box.h"
#include "ui/ui_utility.h"
#include "ui/widgets/buttons.h"
#include "ui/wrap/vertical_layout.h"
#include "styles/style_layers.h"
#include "styles/style_menu_icons.h"
#include "styles/style_settings.h"

namespace Settings {
using namespace Builder;
namespace {

void OfferRestart(Window::SessionController *controller) {
	controller->show(Ui::MakeConfirmBox({
		.text = tr::lng_settings_need_restart(),
		.confirmed = [] { Core::Restart(); },
		.confirmText = tr::lng_settings_restart_now(),
		.cancelText = tr::lng_settings_restart_later(),
	}));
}

void AddForkToggle(
		SectionBuilder &builder,
		const QString &id,
		rpl::producer<QString> title,
		Fn<bool()> current,
		Fn<void(bool)> set,
		QStringList keywords) {
	const auto button = builder.addButton({
		.id = id,
		.title = std::move(title),
		.st = &st::settingsButtonNoIcon,
		.toggled = rpl::single(current()),
		.keywords = std::move(keywords),
	});
	if (button) {
		button->toggledValue(
		) | rpl::filter([=](bool value) {
			return (value != current());
		}) | rpl::on_next([=](bool value) {
			set(value);
			Core::App().saveSettingsDelayed();
		}, button->lifetime());
	}
}

void BuildWidgetsSection(
		SectionBuilder &builder,
		Window::SessionController *controller) {
	builder.addSubsectionTitle({
		.id = u"ekzegram/widgets"_q,
		.title = tr::lng_settings_widgets_title(),
		.keywords = { u"widgets"_q, u"html"_q, u"bots"_q },
	});

	const auto widgets = builder.addButton({
		.id = u"ekzegram/disable_widgets"_q,
		.title = tr::lng_settings_disable_widgets(),
		.st = &st::settingsButtonNoIcon,
		.toggled = rpl::single(
			Core::App().settings().fork().widgetsDisabled()),
		.keywords = { u"widgets"_q, u"html"_q, u"bots"_q },
	});
	if (widgets) {
		widgets->toggledValue(
		) | rpl::filter([=](bool disabled) {
			return (disabled
				!= Core::App().settings().fork().widgetsDisabled());
		}) | rpl::on_next([=](bool disabled) {
			Core::App().settings().fork().setWidgetsDisabled(disabled);
			Core::App().saveSettingsDelayed();
			OfferRestart(controller);
		}, widgets->lifetime());
	}
	builder.addDividerText(tr::lng_settings_disable_widgets_about());
}

void BuildPasscodeSection(SectionBuilder &builder) {
	builder.addSkip();
	builder.addSubsectionTitle({
		.id = u"ekzegram/passcode"_q,
		.title = tr::lng_settings_passcode_title(),
		.keywords = { u"passcode"_q, u"lock"_q },
	});

	AddForkToggle(
		builder,
		u"ekzegram/auto_submit_passcode"_q,
		tr::lng_settings_auto_submit_passcode(),
		[] { return Core::App().settings().fork().autoSubmitPasscode(); },
		[](bool value) {
			Core::App().settings().fork().setAutoSubmitPasscode(value);
		},
		{ u"passcode"_q, u"lock"_q, u"auto"_q });
	builder.addDividerText(tr::lng_settings_auto_submit_passcode_about());
}

void BuildBehaviorSection(SectionBuilder &builder) {
	builder.addSkip();
	builder.addSubsectionTitle({
		.id = u"ekzegram/behavior"_q,
		.title = tr::lng_settings_behavior_title(),
		.keywords = { u"behavior"_q },
	});

	AddForkToggle(
		builder,
		u"ekzegram/audio_fade"_q,
		tr::lng_settings_audio_fade(),
		[] { return Core::App().settings().fork().audioFade(); },
		[](bool value) {
			Core::App().settings().fork().setAudioFade(value);
		},
		{ u"audio"_q, u"fade"_q, u"sound"_q });

	AddForkToggle(
		builder,
		u"ekzegram/last_seen_in_dialogs"_q,
		tr::lng_settings_last_seen_in_dialogs(),
		[] { return Core::App().settings().fork().lastSeenInDialogs(); },
		[](bool value) {
			Core::App().settings().fork().setLastSeenInDialogs(value);
		},
		{ u"last"_q, u"seen"_q, u"dialogs"_q });

	AddForkToggle(
		builder,
		u"ekzegram/copy_login_code"_q,
		tr::lng_settings_auto_copy_login_codes(),
		[] { return Core::App().settings().fork().copyLoginCode(); },
		[](bool value) {
			Core::App().settings().fork().setCopyLoginCode(value);
		},
		{ u"login"_q, u"code"_q, u"copy"_q, u"clipboard"_q });
	builder.addDividerText(tr::lng_settings_auto_copy_login_codes_about());
}

[[nodiscard]] QStringList CurrentEmojiSearchCodes() {
	const auto chosen = Core::App().settings().fork().emojiSearchLanguages();
	return chosen.split(',', Qt::SkipEmptyParts);
}

[[nodiscard]] QString EmojiSearchLanguagesLabel() {
	const auto codes = CurrentEmojiSearchCodes();
	if (codes.isEmpty()) {
		return tr::lng_settings_emoji_search_languages_automatic(tr::now);
	}
	auto names = QStringList();
	for (const auto &code : codes) {
		names.push_back(Ui::LanguageName(LanguageId::FromName(code)));
	}
	return (names.size() > 2)
		? tr::lng_languages_count(tr::now, lt_count, names.size())
		: names.join(u", "_q);
}

void ShowEmojiSearchLanguagesBox(
		Window::SessionController *controller) {
	auto selected = std::vector<LanguageId>();
	for (const auto &code : CurrentEmojiSearchCodes()) {
		const auto id = LanguageId::FromName(code);
		if (id.known()) {
			selected.push_back(id);
		}
	}
	controller->show(Box(
		Ui::ChooseLanguageBox,
		tr::lng_settings_emoji_search_languages(),
		[=](std::vector<LanguageId> &&list) {
			auto codes = QStringList();
			for (const auto &id : list) {
				codes.push_back(id.twoLetterCode());
			}
			codes.sort();
			Core::App().settings().fork().setEmojiSearchLanguages(
				codes.join(','));
			Core::App().saveSettingsDelayed();
			Core::App().emojiKeywords().refresh();
		},
		std::move(selected),
		true,
		[](LanguageId) { return true; }));
}

void BuildEmojiSection(
		SectionBuilder &builder,
		Window::SessionController *controller) {
	builder.addSkip();
	builder.addSubsectionTitle({
		.id = u"ekzegram/emoji"_q,
		.title = tr::lng_settings_emoji_search_title(),
		.keywords = { u"emoji"_q, u"search"_q, u"languages"_q },
	});

	builder.addButton({
		.id = u"ekzegram/emoji_search_languages"_q,
		.title = tr::lng_settings_emoji_search_languages(),
		.st = &st::settingsButtonNoIcon,
		.label = rpl::merge(
			rpl::single(QString()),
			Core::App().settings().fork().changes()
				| rpl::map([] { return QString(); })
		) | rpl::map([] { return EmojiSearchLanguagesLabel(); }),
		.onClick = [=] { ShowEmojiSearchLanguagesBox(controller); },
		.keywords = { u"emoji"_q, u"search"_q, u"languages"_q },
	});
	builder.addDividerText(tr::lng_settings_emoji_search_languages_about());
}

void BuildMenuSection(
		SectionBuilder &builder,
		Window::SessionController *controller) {
	builder.addSkip();
	builder.addSubsectionTitle({
		.id = u"ekzegram/menu"_q,
		.title = tr::lng_settings_menu_title(),
		.keywords = { u"menu"_q, u"search"_q, u"markdown"_q },
	});

	const auto search = builder.addButton({
		.id = u"ekzegram/search_selected"_q,
		.title = tr::lng_settings_search_engine(),
		.st = &st::settingsButtonNoIcon,
		.keywords = { u"search"_q, u"engine"_q, u"selected"_q },
	});
	if (search) {
		const auto state = search->lifetime().make_state<rpl::variable<bool>>(
			Core::App().settings().fork().searchSelected());
		search->toggleOn(state->value());
		search->toggledChanges() | rpl::on_next([=](bool enabled) {
			if (enabled == state->current()) {
				return;
			} else if (!enabled) {
				*state = false;
				Core::App().settings().fork().setSearchSelected(false);
				Core::App().saveSettingsDelayed();
				return;
			}
			*state = true;
			controller->show(
				Fork::MakeSearchEngineBox([=](bool ok) {
					Core::App().settings().fork().setSearchSelected(ok);
					Core::App().saveSettingsDelayed();
					if (!ok) {
						*state = false;
					}
				}),
				Ui::LayerOption::KeepOther);
		}, search->lifetime());
	}

	AddForkToggle(
		builder,
		u"ekzegram/remember_media"_q,
		tr::lng_settings_remember_media_menu(),
		[] {
			return Core::App().settings().fork().addToMenuRememberMedia();
		},
		[](bool value) {
			Core::App().settings().fork().setAddToMenuRememberMedia(value);
		},
		{ u"media"_q, u"remember"_q, u"menu"_q });

	builder.addButton({
		.id = u"ekzegram/markdown_clipboard"_q,
		.title = tr::lng_settings_markdown_clipboard_text(),
		.st = &st::settingsButtonNoIcon,
		.onClick = [=] {
			controller->show(Menu::MakeMarkdownClipboardTextBox());
		},
		.keywords = { u"markdown"_q, u"clipboard"_q, u"save"_q },
	});
}

void BuildLinksSection(
		SectionBuilder &builder,
		Window::SessionController *controller) {
	builder.addSkip();
	builder.addSubsectionTitle({
		.id = u"ekzegram/links"_q,
		.title = tr::lng_settings_links_title(),
		.keywords = { u"links"_q, u"paste"_q, u"uri"_q },
	});

	builder.addButton({
		.id = u"ekzegram/link_replacements"_q,
		.title = tr::lng_fork_link_replacements(),
		.st = &st::settingsButtonNoIcon,
		.label = rpl::single(rpl::empty_value()) | rpl::then(
			Forkgram::LinkReplacements::Changes()
		) | rpl::map([] {
			return Forkgram::LinkReplacements::Enabled()
				? QString::number(Forkgram::LinkReplacements::EnabledCount())
				: tr::lng_settings_cloud_password_off(tr::now);
		}),
		.onClick = [=] {
			controller->show(Box(Forkgram::LinkReplacementsBox));
		},
		.keywords = { u"link"_q, u"replace"_q, u"paste"_q, u"regex"_q },
	});

	const auto uri = builder.addButton({
		.id = u"ekzegram/uri_scheme"_q,
		.title = tr::lng_settings_uri_scheme(),
		.st = &st::settingsButtonNoIcon,
		.keywords = { u"uri"_q, u"scheme"_q, u"custom"_q },
	});
	if (uri) {
		const auto state = uri->lifetime().make_state<rpl::variable<bool>>(
			Core::App().settings().fork().askUriScheme());
		uri->toggleOn(state->value());
		uri->toggledChanges() | rpl::on_next([=](bool enabled) {
			if (enabled == state->current()) {
				return;
			} else if (!enabled) {
				*state = false;
				Core::App().settings().fork().setAskUriScheme(false);
				Core::App().saveSettingsDelayed();
				return;
			}
			*state = true;
			controller->show(
				Box(Forkgram::UriSchemeBox, [=](bool ok) {
					Core::App().settings().fork().setAskUriScheme(ok);
					Core::App().saveSettingsDelayed();
					if (!ok) {
						*state = false;
					}
				}),
				Ui::LayerOption::KeepOther);
		}, uri->lifetime());
	}
	builder.addDividerText(tr::lng_fork_link_replacements_about());
}

} // namespace

class Fork : public Section<Fork> {
public:
	Fork(
		QWidget *parent,
		Window::SessionController *controller);

	[[nodiscard]] rpl::producer<QString> title() override;

private:
	void setupContent();

};

const auto kMeta = BuildHelper({
	.id = Fork::Id(),
	.parentId = MainId(),
	.title = &tr::lng_settings_section_ekzegram,
	.icon = &st::menuIconCustomize,
}, [](SectionBuilder &builder) {
	const auto controller = builder.controller();
	BuildWidgetsSection(builder, controller);
	BuildPasscodeSection(builder);
	BuildBehaviorSection(builder);
	BuildLinksSection(builder, controller);
	BuildMenuSection(builder, controller);
	BuildEmojiSection(builder, controller);
});

const SectionBuildMethod kForkSection = kMeta.build;

Fork::Fork(
	QWidget *parent,
	Window::SessionController *controller)
: Section(parent, controller) {
	setupContent();
}

rpl::producer<QString> Fork::title() {
	return tr::lng_settings_section_ekzegram();
}

void Fork::setupContent() {
	const auto content = Ui::CreateChild<Ui::VerticalLayout>(this);

	build(content, kForkSection);

	Ui::ResizeFitChild(this, content);
}

Type ForkId() {
	return Fork::Id();
}

} // namespace Settings
