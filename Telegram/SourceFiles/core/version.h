/*
This file is part of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/telegramdesktop/tdesktop/blob/master/LEGAL
*/
#pragma once

#include "base/const_string.h"

#define TDESKTOP_REQUESTED_ALPHA_VERSION (0ULL)

#ifdef TDESKTOP_ALLOW_CLOSED_ALPHA
#define TDESKTOP_ALPHA_VERSION TDESKTOP_REQUESTED_ALPHA_VERSION
#else // TDESKTOP_ALLOW_CLOSED_ALPHA
#define TDESKTOP_ALPHA_VERSION (0ULL)
#endif // TDESKTOP_ALLOW_CLOSED_ALPHA

// used in Updater.cpp and Setup.iss for Windows
// FORK (ekzegram): fork-specific AppId GUID (was upstream's), so ekzegram
// and official Telegram Desktop coexist cleanly (separate uninstall
// entries, shortcuts and registry identity).
constexpr auto AppId = "{4DAB0BDC-F97C-4158-9BF1-3C42796CDA4A}"_cs;
constexpr auto AppNameOld = "Telegram Win (Unofficial)"_cs;
constexpr auto AppName = "ekzegram"_cs;
constexpr auto AppFile = "Telegram"_cs;
constexpr auto AppVersion = 7004007;
// FORK (ekzegram): the ekzegram release number this source tree ships as.
// Must match the suffix of the release tag "v<upstream>-ekzegram<N>"; the
// Make release workflow verifies the tag against this value, and the fork
// update checker compares it with newer releases on GitHub.
constexpr auto ForkBuildNumber = 1;
constexpr auto AppVersionStr = "7.4.7";
constexpr auto AppBetaVersion = false;
constexpr auto AppAlphaVersion = TDESKTOP_ALPHA_VERSION;
