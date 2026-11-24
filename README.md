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

* `ekzegram-linux.tar.xz` — Linux (x86-64)
* `ekzegram-windows.zip` — Windows (x64), portable (data beside the exe)
* `ekzegram-setup.exe` — Windows (x64), installer (data in `%APPDATA%\ekzegram`)
* `ekzegram-macos-arm64.dmg` — macOS (Apple Silicon)
* `ekzegram-macos-x86_64.dmg` — macOS (Intel)

The application is branded **ekzegram** (its own name and icon) and keeps its profile
separately from official Telegram Desktop.

## Builds

* CI builds **Release** binaries for **Linux, Windows (x64) and macOS** on every push —
  see the [Actions][actions] page (artifacts are kept 30 days).
* Builds use `DESKTOP_APP_DISABLE_AUTOUPDATE=ON`, since the official updater would
  replace this fork with the official binary. Instead, portable installs show a
  one-time in-app notice when a new release appears on GitHub (it opens the release
  page — nothing is downloaded automatically); package-managed installs (pacman)
  are updated by their package manager and get no notice.
* API credentials are **not** included: building requires your own `api_id`/`api_hash`
  ([how to obtain][api_credentials]), passed via `TDESKTOP_API_ID`/`TDESKTOP_API_HASH`
  repository secrets (CI) or `~/.config/tdesktop-fork/api.conf` (local `build-fork.sh`).

## Data locations & portable mode

| Where you run ekzegram from | Your data (`tdata`) lives in |
|---|---|
| Windows portable zip | `tdata` next to `ekzegram.exe` |
| Windows installer | `%APPDATA%\ekzegram\tdata` |
| macOS (DMG) | `~/Library/Application Support/ekzegram` |
| Linux (tarball or pacman) | `~/.local/share/ekzegram` |

`tdata` holds your account session, drafts and secret chats — back it up, and don't
keep it in a cloud-synced folder (sync corrupts it and uploads your chats). If the
Windows portable folder isn't writable (e.g. Program Files), the app silently falls
back to `%APPDATA%\ekzegram` instead. Advanced: `-workdir <dir>` or the
`TDESKTOP_WORKDIR` environment variable overrides everything. Migrating between the
Windows flavors is just moving `tdata` — see `packaging/windows/README.txt`.

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

## Credits

* [Telegram Desktop][tdesktop] — the official client this fork is based on.
* [Forkgram][forkgram] by [23rd][23rd] — several convenience features (fork
  settings, widget kill switch, message stash, the message-deletion effect and
  others) are adapted from Forkgram and remain under the same GPLv3 license.
  Thanks to the Forkgram maintainers and contributors.

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

Готовые бинарники — на странице [Releases][releases]: Linux (x86-64), Windows (x64 —
портативная zip и установщик `ekzegram-setup.exe`), macOS (Apple Silicon и Intel, DMG).
Данные (`tdata`): Windows portable — рядом с `ekzegram.exe`; установщик —
`%APPDATA%\ekzegram`; macOS — `~/Library/Application Support/ekzegram`; Linux —
`~/.local/share/ekzegram`.

### Сборка

* CI собирает **Release**-бинарники для **Linux, Windows (x64) и macOS** при каждом
  push — см. страницу [Actions][actions].
* Сборки используют `DESKTOP_APP_DISABLE_AUTOUPDATE=ON`: официальный апдейтер заменил бы
  бинарник форка на официальный. Вместо этого портативные сборки показывают разовое
  уведомление о новом релизе на GitHub (оно открывает страницу релиза, ничего не
  скачивая); пакетные сборки обновляются пакетным менеджером.
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

### Благодарности

* [Telegram Desktop][tdesktop] — официальный клиент, на котором основан форк.
* [Forkgram][forkgram] от [23rd][23rd] — ряд удобных функций (fork-настройки,
  отключение виджетов, stash сообщений, эффект удаления и другие) адаптированы из
  Forkgram и распространяются по той же лицензии GPLv3. Спасибо авторам и
  участникам Forkgram.

---

[tdesktop]: https://github.com/telegramdesktop/tdesktop
[forkgram]: https://github.com/forkgram/tdesktop
[23rd]: https://github.com/23rd
[releases]: https://github.com/nyakze/ekzegram/releases
[actions]: https://github.com/nyakze/ekzegram/actions
[api_credentials]: docs/api_credentials.md
