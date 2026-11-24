<p align="center">
  <img src="docs/assets/banner.png" width="820" alt="ekzegram — an unofficial Telegram Desktop fork">
</p>

<p align="center">
  <a href="https://github.com/nyakze/ekzegram/releases"><img alt="latest release" src="https://img.shields.io/github/v/release/nyakze/ekzegram?style=flat-square"></a>
  <img alt="Linux x86-64" src="https://img.shields.io/badge/Linux-x86--64-2E9EB4?style=flat-square&logo=linux&logoColor=white">
  <img alt="Windows x64" src="https://img.shields.io/badge/Windows-x64-2E9EB4?style=flat-square&logo=windows&logoColor=white">
  <img alt="macOS arm64 and x86-64" src="https://img.shields.io/badge/macOS-arm64%20%C2%B7%20x86--64-2E9EB4?style=flat-square&logo=apple&logoColor=white">
  <a href="LICENSE"><img alt="license: GPLv3 with OpenSSL exception" src="https://img.shields.io/badge/license-GPL--3.0-2E9EB4?style=flat-square"></a>
</p>

<p align="center">
  <a href="#features">Features</a> &nbsp;·&nbsp;
  <a href="#download">Download</a> &nbsp;·&nbsp;
  <a href="#building">Building</a>
</p>

**ekzegram** is an unofficial fork of [Telegram Desktop][tdesktop] — the same fast
native client with its own name, icons and ready-to-use builds, plus a small set of
convenience features and fixes on top. It follows upstream `dev` closely: everything
else is stock.

## Features

All fork extras are gathered in a dedicated **ekzegram** section of Settings; the rest
of the app looks and behaves like the official client.

* **Widget kill switch** — show a placeholder instead of bot HTML widgets (Mini Apps
  keep working).
* **Link rewriting** — regex rules that rewrite pasted URLs, with an "undo" toast.
* **Open links externally** — open any link through a custom URI scheme, e.g.
  `potplayer://`.
* **Send as copy** — resend messages as new ones, without the "Forwarded from" header.
* **Message menu extras** — save a selection to a Markdown file, copy it depersonalized
  ("Person 1, Person 2…"), group media into an album, share any message, search the
  selected text, remember/replace/swap media.
* **Draft stash** — Ctrl+S stashes the current draft (text, replies, pending files) and
  restores it later.
* **Quality-of-life tweaks** — auto-copy login codes, passcode that submits itself when
  complete, last-seen time in the chat list, "Mute all", "Go to first message",
  emoji-search languages, up to 30 accounts.
* **Downgrade safety** — if a `tdata` written by a newer version is opened by an older
  one, it is moved intact to `tdata_backup` instead of being corrupted.

## Download

Prebuilt binaries for **Linux (x86-64)**, **Windows (x64)** and **macOS (Apple Silicon
and Intel)** are on the [Releases][releases] page — a portable zip and an installer for
Windows, DMGs for macOS. The app keeps its profile separate from official Telegram
Desktop, so both can be installed side by side.

## Building

```bash
./build-fork.sh
```

The script sets up submodules and the `tdesktop:centos_env` Docker image on first run
and leaves a Release binary at `out/Release/ekzegram`; see
[docs/building-linux.md](docs/building-linux.md) for details. CI builds all three
platforms on every push — see the [Actions][actions] page.

Building needs your own API credentials ([how to obtain][api_credentials]) — the
well-known public ones cause login errors. Pass them as
`TDESKTOP_API_ID`/`TDESKTOP_API_HASH` repository secrets for CI, or put them in
`~/.config/tdesktop-fork/api.conf` for local builds.

The fork is rebased onto upstream `dev` regularly, with fork-specific changes kept in a
small commit stack so rebases stay cheap.

## Credits

* [Telegram Desktop][tdesktop] — the official client this fork is based on.
* [Forkgram][forkgram] by [23rd][23rd] — several features are adapted from Forkgram and
  remain under the same GPLv3 license. Thanks to the Forkgram maintainers and
  contributors.

## Disclaimer

* Unofficial. Not affiliated with or endorsed by Telegram; the name "Telegram" is used
  only to refer to the original product.
* The application collects no user data — see [PRIVACY.md](PRIVACY.md).
* Builds are **unsigned**: on Windows/macOS you may need to allow the app through
  SmartScreen / Gatekeeper on first launch.
* Distributed under the same terms as upstream: GPLv3 with the OpenSSL exception.

---

[tdesktop]: https://github.com/telegramdesktop/tdesktop
[forkgram]: https://github.com/forkgram/tdesktop
[23rd]: https://github.com/23rd
[releases]: https://github.com/nyakze/ekzegram/releases
[actions]: https://github.com/nyakze/ekzegram/actions
[api_credentials]: docs/api_credentials.md
