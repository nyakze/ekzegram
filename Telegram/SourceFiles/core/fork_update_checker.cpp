/*
This file is part of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/telegramdesktop/tdesktop/blob/master/LEGAL
*/
#include "core/fork_update_checker.h"

#include "core/application.h"
#include "core/core_settings.h"
#include "core/version.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QDateTime>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QRegularExpression>
#include <QtCore/QTimer>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>

#include <memory>
#include <optional>

#ifndef Q_OS_WIN
#ifndef Q_OS_MACOS
#include <ksandbox.h>
#endif // !Q_OS_MACOS
#endif // !Q_OS_WIN

namespace Core {
namespace ForkUpdate {
namespace {

constexpr auto kFirstCheckDelay = 30 * 1000;
constexpr auto kRecheckInterval = 24 * 60 * 60 * 1000;
constexpr auto kMinIntervalSec = qint64(23 * 60 * 60);

[[nodiscard]] rpl::event_stream<Info> &Found() {
	static auto result = rpl::event_stream<Info>();
	return result;
}

[[nodiscard]] rpl::event_stream<bool> &CheckedNoUpdate() {
	static auto result = rpl::event_stream<bool>();
	return result;
}

[[nodiscard]] bool PortableInstall() {
#if defined Q_OS_WIN || defined Q_OS_MACOS
	return true;
#else // Q_OS_WIN || Q_OS_MACOS
	if (KSandbox::isInside() || KSandbox::isSnap()) {
		return false;
	}
	const auto path = QCoreApplication::applicationFilePath();
	return !path.startsWith(u"/usr/"_q) && !path.startsWith(u"/opt/"_q);
#endif // Q_OS_WIN || Q_OS_MACOS
}

struct Version {
	int major = 0;
	int minor = 0;
	int patch = 0;
	int fork = 0;
};

// Parses tags like "v7.4.6-ekzegram3", ignoring everything else
// (drafts, pre-releases and unrelated tags simply don't match).
[[nodiscard]] std::optional<Version> ParseTag(const QString &tag) {
	static const auto pattern = QRegularExpression(
		u"^v(\\d+)\\.(\\d+)\\.(\\d+)-ekzegram(\\d+)$"_q);
	const auto match = pattern.match(tag);
	if (!match.hasMatch()) {
		return std::nullopt;
	}
	return Version{
		.major = match.captured(1).toInt(),
		.minor = match.captured(2).toInt(),
		.patch = match.captured(3).toInt(),
		.fork = match.captured(4).toInt(),
	};
}

[[nodiscard]] bool NewerThanCurrent(const Version &version) {
	const auto current = Version{
		.major = AppVersion / 1000000,
		.minor = (AppVersion / 1000) % 1000,
		.patch = AppVersion % 1000,
		.fork = ForkBuildNumber,
	};
	if (version.major != current.major) {
		return version.major > current.major;
	} else if (version.minor != current.minor) {
		return version.minor > current.minor;
	} else if (version.patch != current.patch) {
		return version.patch > current.patch;
	}
	return version.fork > current.fork;
}

class Checker final : public QObject {
public:
	Checker();
	void checkNow();

private:
	void scheduleNext(int delayMs);
	void perform(bool manual);

	QNetworkAccessManager _manager;
	rpl::lifetime _lifetime;

};

Checker::Checker() {
	scheduleNext(kFirstCheckDelay);

	Core::App().settings().checkForUpdatesChanges(
	) | rpl::filter([=](bool enabled) {
		return enabled;
	}) | rpl::on_next([=] {
		scheduleNext(kFirstCheckDelay);
	}, _lifetime);
}

void Checker::scheduleNext(int delayMs) {
	QTimer::singleShot(delayMs, this, [=] {
		const auto &settings = Core::App().settings();
		if (!settings.checkForUpdates()) {
			scheduleNext(kRecheckInterval);
			return;
		}
		const auto now = QDateTime::currentSecsSinceEpoch();
		if (now - settings.updateCheckLastTime() < kMinIntervalSec) {
			scheduleNext(kRecheckInterval);
			return;
		}
		perform(false);
	});
}

void Checker::checkNow() {
	perform(true);
}

void Checker::perform(bool manual) {
	auto request = QNetworkRequest(
		QUrl(u"https://api.github.com/repos/nyakze/ekzegram/releases/latest"_q));
	request.setRawHeader("User-Agent", "ekzegram-update-check");
	request.setRawHeader("Accept", "application/vnd.github+json");
	const auto reply = _manager.get(request);
	QObject::connect(reply, &QNetworkReply::finished, this, [=] {
		reply->deleteLater();
		const auto data = reply->readAll();
		const auto error = reply->error();
		auto &settings = Core::App().settings();
		settings.setUpdateCheckLastTime(QDateTime::currentSecsSinceEpoch());
		scheduleNext(kRecheckInterval);
		if (error != QNetworkReply::NoError) {
			if (manual) {
				CheckedNoUpdate().fire(false);
			}
			Core::App().saveSettingsDelayed();
			return;
		}
		const auto object = QJsonDocument::fromJson(data).object();
		const auto tag = object.value(u"tag_name"_q).toString();
		const auto parsed = ParseTag(tag);
		if (parsed && NewerThanCurrent(*parsed)) {
			if (manual || settings.updateCheckSeenTag() != tag) {
				settings.setUpdateCheckSeenTag(tag);
				Found().fire({
					.tag = tag,
					.url = object.value(u"html_url"_q).toString(),
				});
			}
		} else if (manual) {
			CheckedNoUpdate().fire(true);
		}
		Core::App().saveSettingsDelayed();
	});
}

} // namespace

[[nodiscard]] std::unique_ptr<Checker> &Instance() {
	static auto result = std::unique_ptr<Checker>();
	return result;
}

bool Available() {
	return PortableInstall();
}

void Start() {
	if (!Available() || Instance()) {
		return;
	}
	Instance() = std::make_unique<Checker>();
}

void CheckNow() {
	if (!Available()) {
		return;
	}
	if (!Instance()) {
		Instance() = std::make_unique<Checker>();
	}
	Instance()->checkNow();
}

rpl::producer<Info> found() {
	return Found().events();
}

rpl::producer<bool> checkedNoUpdate() {
	return CheckedNoUpdate().events();
}

} // namespace ForkUpdate
} // namespace Core
