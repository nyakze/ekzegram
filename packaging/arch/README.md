# Arch packaging (ekzegram-bin)

A pacman package that repackages the prebuilt `ekzegram-linux.tar.gz` from
this fork's GitHub Releases. No compilation required.

## How updates work

1. You publish a GitHub release (e.g. `v7.4.6-ekzegram1`) — the existing
   release workflow already uploads `ekzegram-linux.tar.xz` + `SHA256SUMS.txt`
   (older releases used `.tar.gz`; the workflow falls back automatically).
2. The `arch-repo` workflow (`.github/workflows/arch-repo.yml`) triggers on
   the release, builds `ekzegram-bin` in an Arch container, and publishes it
   to the `arch-repo` release as static assets — a pacman repository at
   `https://github.com/nyakze/ekzegram/releases/download/arch-repo`.
   (Release assets allow up to 2 GB per file; git blocks files over 100 MB,
   which rules out hosting the package in a branch.)
3. On your machine, `pacman -Syu` (or `paru`) picks up the new version like
   any other package.

No AUR account is involved anywhere: an AUR login is only needed to *publish*
PKGBUILDs to aur.archlinux.org.

## One-time setup on the machine (needs root)

Add to `/etc/pacman.conf` (e.g. right above `[core]`):

```ini
[ekzegram]
SigLevel = Never
Server = https://github.com/nyakze/ekzegram/releases/download/arch-repo
```

Then:

```sh
sudo pacman -Syu          # or just: paru
```

`SigLevel = Never` because the packages are unsigned; traffic still goes
over HTTPS from your own GitHub release.

## Building manually

```sh
cd packaging/arch
makepkg -si
```

Bump `pkgver` first if you want a newer release (tag `vX-Y` maps to
`pkgver=X_Y`). Checksums for the release artifacts are verified against the
release's `SHA256SUMS.txt` in `prepare()`, so no checksum bumps are needed.

## Installed layout

- `/usr/bin/ekzegram` — the binary
- `/usr/share/applications/ekzegram.desktop`
- `/usr/share/icons/hicolor/*/apps/ekzegram.png`
- `/usr/share/icons/hicolor/symbolic/apps/ekzegram{,-attention,-mute}-symbolic.svg`

## Monochrome tray icon (KDE Plasma)

The package ships three symbolic icons whose names match what the app looks
up (`Platform::PanelIconName`, e.g. `ekzegram-symbolic`, plus `-attention`
for unread and `-mute` for muted). Plasma renders icon theme names ending in
`-symbolic` as monochrome panel icons colored by the panel theme, so the tray
icon is monochrome out of the box. Because the icons are present, the
in-app toggle *Settings → Advanced → Use monochrome icon* also appears on
Linux (it is hidden otherwise); it defaults to on.

The upstream binary copies these icons into `~/.local/share/icons` by itself,
but that self-install is disabled in this fork (the updater is off), hence
the packaging provides them. For a non-pacman install, copy the three SVGs
from `Telegram/Resources/icons/tray_monochrome*.svg` into
`~/.local/share/icons/hicolor/symbolic/apps/` under the names above.
