# Privacy Policy

**ekzegram** is an unofficial fork of Telegram Desktop.

**The application does not collect any personal data.**

* The developer of this fork collects, stores, or processes **no user data whatsoever**. There is no telemetry, no analytics, no advertising, and no external services belonging to the developer.
* Crash reporting is disabled at build time (`DESKTOP_APP_DISABLE_CRASH_REPORTS=ON`).
* The official auto-updater is disabled at build time (`DESKTOP_APP_DISABLE_AUTOUPDATE=ON`): the application never downloads or installs updates by itself.
* Portable builds do check GitHub for new releases, at most once a day: one anonymous request to the GitHub Releases API (`api.github.com`). The request contains no personal data — GitHub only sees a regular HTTPS request, such as your IP address. When a new release is found, the application just shows a notice that opens the release page; nothing is downloaded automatically. The check can be turned off in Settings → Advanced, and package-managed installs never perform it.
* All messaging functionality works through **Telegram's servers**. Any data you send or receive (messages, files, phone number, contacts) is processed according to [Telegram's Privacy Policy](https://telegram.org/privacy). This fork is not affiliated with Telegram and has no access to your data beyond what the official client has.
* Your account data (session keys, settings, local cache) is stored **locally** on your device and is never transmitted anywhere by the fork itself.

If you have questions, open an issue: https://github.com/nyakze/ekzegram/issues
