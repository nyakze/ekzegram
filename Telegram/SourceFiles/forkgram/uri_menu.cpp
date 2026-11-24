/*
This file is part of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/telegramdesktop/tdesktop/blob/master/LEGAL

Adapted from Forkgram (https://github.com/forkgram/tdesktop).
*/

#include "forkgram/uri_menu.h"

#include "core/application.h"
#include "core/core_settings.h"
#include "forkgram/uri_open.h"
#include "lang/lang_keys.h"
#include "ui/layers/generic_box.h"
#include "ui/widgets/fields/input_field.h"
#include "ui/widgets/popup_menu.h"

#include "styles/style_boxes.h"
#include "styles/style_widgets.h"

namespace Forkgram {

void FillUrlWithCustomUri(
		not_null<Ui::PopupMenu*> menu,
		const QString &url) {
	if (!Core::App().settings().fork().askUriScheme()) {
		return;
	}
	auto callback = [=] {
		OpenUrlWithCustomUri(Core::App().settings().fork().uriScheme() + url);
	};
	menu->addAction(
		tr::lng_context_open_uri_link(tr::now),
		std::move(callback));
}

void UriSchemeBox(not_null<Ui::GenericBox*> box, Fn<void(bool)> done) {
	Expects(done != nullptr);

	box->setTitle(tr::lng_settings_uri_scheme_box_title());
	const auto url = box->addRow(
		object_ptr<Ui::InputField>(
			box,
			st::defaultInputField,
			tr::lng_settings_uri_scheme_field_label(),
			Core::App().settings().fork().uriScheme()),
		st::markdownLinkFieldPadding);
	const auto submit = [=] {
		const auto value = url->getLastText();
		if (value.indexOf(u"://"_q) < 2) {
			url->showError();
			return;
		}
		Core::App().settings().fork().setUriScheme(value);
		Core::App().saveSettings();
		done(true);
		box->closeBox();
	};
	url->submits() | rpl::on_next([=] {
		submit();
	}, url->lifetime());
	box->setFocusCallback([=] {
		url->setFocusFast();
	});
	box->addButton(tr::lng_box_ok(), submit);
	box->addButton(tr::lng_cancel(), [=] {
		done(!Core::App().settings().fork().uriScheme().isEmpty());
		box->closeBox();
	});
}

} // namespace Forkgram
