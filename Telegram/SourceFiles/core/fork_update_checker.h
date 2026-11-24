/*
This file is part of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/telegramdesktop/tdesktop/blob/master/LEGAL
*/
#pragma once

#include <rpl/rpl.h>

#include <QtCore/QString>

namespace Core {
namespace ForkUpdate {

struct Info {
	QString tag;
	QString url;
};

// FORK (ekzegram): the check runs for portable installs only — installs
// managed by a package manager (pacman, AUR, Flatpak, Snap) are updated
// through it and must not be nagged about updates at all.
[[nodiscard]] bool Available();

// Schedules periodic checks, does nothing when !Available().
void Start();

[[nodiscard]] rpl::producer<Info> found();

} // namespace ForkUpdate
} // namespace Core
