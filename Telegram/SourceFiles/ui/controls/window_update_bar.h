/*
This file is part of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/telegramdesktop/tdesktop/blob/master/LEGAL
*/
#pragma once

#include "base/object_ptr.h"

namespace Ui {

class RpWidget;

// FORK (ekzegram): persistent update notice, modeled on the outdated bar.
[[nodiscard]] object_ptr<RpWidget> CreateUpdateBar(
	not_null<QWidget*> parent,
	const QString &tag,
	Fn<void()> open,
	Fn<void()> close);

} // namespace Ui
