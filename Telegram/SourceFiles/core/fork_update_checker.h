/*
This file is part of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/telegramdesktop/tdesktop/blob/master/LEGAL
*/
#pragma once

#include <optional>

#include <rpl/rpl.h>

#include <QtCore/QString>

namespace Core {
namespace ForkUpdate {

struct Info {
	QString tag;
	QString url;
	bool manual = false;
};

// FORK (ekzegram): the update remembered from the last check, if any.
[[nodiscard]] std::optional<Info> AvailableUpdate();

// FORK (ekzegram): the check runs for portable installs only — installs
// managed by a package manager (pacman, AUR, Flatpak, Snap) are updated
// through it and must not be nagged about updates at all.
[[nodiscard]] bool Available();

// Schedules periodic checks, does nothing when !Available().
void Start();

// Performs a check immediately, ignoring the 24 hour throttle.
void CheckNow();

// Performs a background check immediately (no dialogs), for example
// after the user changes the update settings.
void Recheck();

[[nodiscard]] rpl::producer<Info> found();

// Fired when a manual CheckNow() completes without an update to install.
// The bool is false when the check itself failed (network/server error).
[[nodiscard]] rpl::producer<bool> checkedNoUpdate();

} // namespace ForkUpdate
} // namespace Core
