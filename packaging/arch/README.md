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

- `/opt/ekzegram/ekzegram` — the binary (as before)
- `/usr/bin/ekzegram` — symlink to the binary
- `/usr/share/applications/ekzegram.desktop`, `/usr/share/icons/hicolor/*/apps/ekzegram.png`
