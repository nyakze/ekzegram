/*
This file is part of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/telegramdesktop/tdesktop/blob/master/LEGAL

Adapted from Forkgram (https://github.com/forkgram/tdesktop).
*/
#pragma once

#include "base/basic_types.h"

namespace Ui {
class GenericBox;
class PopupMenu;
} // namespace Ui

namespace Forkgram {

void FillUrlWithCustomUri(
	not_null<Ui::PopupMenu*> menu,
	const QString &url);

void UriSchemeBox(not_null<Ui::GenericBox*> box, Fn<void(bool)> done);

} // namespace Forkgram
