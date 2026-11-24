/*
This file is part of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/telegramdesktop/tdesktop/blob/master/LEGAL
*/
#include "settings/sections/settings_fork.h"

#include "settings/settings_common_session.h"
#include "core/application.h"
#include "core/core_settings.h"
#include "lang/lang_keys.h"
#include "settings/settings_builder.h"
#include "ui/boxes/confirm_box.h"
#include "ui/ui_utility.h"
#include "ui/wrap/vertical_layout.h"
#include "styles/style_layers.h"
#include "styles/style_menu_icons.h"
#include "styles/style_settings.h"

namespace Settings {
namespace {

void OfferRestart(not_null<Window::SessionController*> controller) {
	controller->show(Ui::MakeConfirmBox({
		.text = tr::lng_settings_need_restart(),
		.confirmed = [] { Core::Restart(); },
		.confirmText = tr::lng_settings_restart_now(),
		.cancelText = tr::lng_settings_restart_later(),
	}));
}

void BuildWidgetsSection(
		SectionBuilder &builder,
		not_null<Window::SessionController*> controller) {
	const auto &fork = Core::App().settings().fork();

	builder.addSubsectionTitle({
		.id = u"ekzegram/widgets"_q,
		.title = tr::lng_settings_widgets_title(),
		.keywords = { u"widgets"_q, u"html"_q, u"bots"_q },
	});

	const auto widgets = builder.addButton({
		.id = u"ekzegram/disable_widgets"_q,
		.title = tr::lng_settings_disable_widgets(),
		.st = &st::settingsButtonNoIcon,
		.toggled = rpl::single(fork.widgetsDisabled()),
		.keywords = { u"widgets"_q, u"html"_q, u"bots"_q },
	});
	if (widgets) {
		widgets->toggledValue(
		) | rpl::filter([=](bool disabled) {
			return (disabled != Core::App().settings().fork().widgetsDisabled());
		}) | rpl::on_next([=](bool disabled) {
			Core::App().settings().fork().setWidgetsDisabled(disabled);
			Core::App().saveSettingsDelayed();
			OfferRestart(controller);
		}, widgets->lifetime());
	}
	builder.addDividerText(tr::lng_settings_disable_widgets_about());
}

void BuildPasscodeSection(SectionBuilder &builder) {
	builder.addDivider();
	const auto autoSubmit = builder.addButton({
		.id = u"ekzegram/auto_submit_passcode"_q,
		.title = tr::lng_settings_auto_submit_passcode(),
		.st = &st::settingsButtonNoIcon,
		.toggled = rpl::single(
			Core::App().settings().fork().autoSubmitPasscode()),
		.keywords = { u"passcode"_q, u"lock"_q, u"auto"_q },
	});
	if (autoSubmit) {
		autoSubmit->toggledValue(
		) | rpl::filter([=](bool enabled) {
			return (enabled
				!= Core::App().settings().fork().autoSubmitPasscode());
		}) | rpl::on_next([=](bool enabled) {
			Core::App().settings().fork().setAutoSubmitPasscode(enabled);
			Core::App().saveSettingsDelayed();
		}, autoSubmit->lifetime());
	}
	builder.addDividerText(tr::lng_settings_auto_submit_passcode_about());
}

class Fork : public Section<Fork> {
public:
	Fork(
		QWidget *parent,
		not_null<Window::SessionController*> controller);

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
});

const SectionBuildMethod kForkSection = kMeta.build;

Fork::Fork(
	QWidget *parent,
	not_null<Window::SessionController*> controller)
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

} // namespace Settings
