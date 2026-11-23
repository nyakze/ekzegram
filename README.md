# ekzegram

**ekzegram** — an unofficial fork of [Telegram Desktop][tdesktop].

Not affiliated with Telegram. Based on the GPLv3 (OpenSSL exception) sources of the official client.

---

## What this is

A personal, branded build of Telegram Desktop that tracks upstream closely and ships
convenient prebuilt binaries. It is functionally the official desktop client, plus a
small set of bug fixes, its own name and icons, and its own release builds.

Everything else — including all upstream fixes and features — is stock.

## Download

Prebuilt binaries are on the [Releases][releases] page:

* `ekzegram-linux.tar.gz` — Linux (x86-64)
* `ekzegram-windows.zip` — Windows (x64)
* `ekzegram-macos-arm64.zip` — macOS (Apple Silicon)
* `ekzegram-macos-x86_64.zip` — macOS (Intel)

The application is branded **ekzegram** (its own name and icon) and keeps its profile
separately from official Telegram Desktop.

## Builds

* CI builds **Release** binaries for **Linux, Windows (x64) and macOS** on every push —
  see the [Actions][actions] page (artifacts are kept 30 days).
* Builds use `DESKTOP_APP_DISABLE_AUTOUPDATE=ON`, since the official updater would
  replace this fork with the official binary. There is no auto-update channel yet —
  watch the repository to hear about new releases.
* API credentials are **not** included: building requires your own `api_id`/`api_hash`
  ([how to obtain][api_credentials]), passed via `TDESKTOP_API_ID`/`TDESKTOP_API_HASH`
  repository secrets (CI) or `~/.config/tdesktop-fork/api.conf` (local `build-fork.sh`).

## Building locally

```
./build-fork.sh
```

The script reads `~/.config/tdesktop-fork/api.conf`, sets up the submodules and the
`tdesktop:centos_env` docker image on first run, and builds a Release binary at
`out/Release/ekzegram`. See [docs/building-linux.md](docs/building-linux.md) for details.

## Maintenance

* The fork is regularly rebased onto upstream `dev`; the fork-specific changes are kept
  in a small, reviewable commit stack (branding + CI) so rebases stay cheap.
* Build your own `api_id`/`api_hash` — the well-known public ones cause login errors.

## Disclaimer

* Unofficial. Not affiliated with or endorsed by Telegram. The name "Telegram" is used
  only to refer to the original product; the application itself is branded as
  ekzegram with its own name and icons.
* The application collects no user data — see [PRIVACY.md](PRIVACY.md).
* Builds are **unsigned** (see the CI notes): on Windows/macOS you may need to allow the
  app through SmartScreen / Gatekeeper on first launch.
* Distributed under the same terms as upstream: GPLv3 with OpenSSL exception.

---

## На русском

**ekzegram** — неофициальный форк [Telegram Desktop][tdesktop].

Это личная сборка Telegram Desktop со своим названием и иконками, которая следит за
апстримом и публикует готовые бинарники. По функциональности это официальный десктопный
клиент плюс небольшие исправления.

### Скачать

Готовые бинарники — на странице [Releases][releases]: Linux (x86-64), Windows (x64),
macOS (Apple Silicon и Intel).

### Сборка

* CI собирает **Release**-бинарники для **Linux, Windows (x64) и macOS** при каждом
  push — см. страницу [Actions][actions].
* Сборки используют `DESKTOP_APP_DISABLE_AUTOUPDATE=ON`: официальный апдейтер заменил бы
  бинарник форка на официальный. Канала автообновления нет — подпишитесь на репозиторий,
  чтобы узнавать о новых релизах.
* API-креды **не включены**: для сборки нужны ваши `api_id`/`api_hash`
  ([как получить][api_credentials]) — через секреты `TDESKTOP_API_ID`/`TDESKTOP_API_HASH`
  (CI) или `~/.config/tdesktop-fork/api.conf` (локальная `build-fork.sh`).

### Дисклеймер

* Неофициальный форк. Не аффилирован с Telegram и не одобряется им. Название "Telegram"
  используется только для обозначения оригинального продукта; само приложение
  брендировано как ekzegram со своим названием и иконками.
* Приложение не собирает персональные данные — см. [PRIVACY.md](PRIVACY.md).
* Бинарники **не подписаны**: в Windows/macOS может понадобиться разрешить запуск через
  SmartScreen / Gatekeeper при первом старте.
* Распространяется на тех же условиях, что и апстрим: GPLv3 с исключением OpenSSL.

---

[tdesktop]: https://github.com/telegramdesktop/tdesktop
[releases]: https://github.com/nyakze/ekzegram/releases
[actions]: https://github.com/nyakze/ekzegram/actions
[api_credentials]: docs/api_credentials.md
