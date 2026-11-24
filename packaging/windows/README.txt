ekzegram (unofficial Telegram Desktop fork) - portable build
===========================================================

Your data lives in the "tdata" folder NEXT TO ekzegram.exe:
message cache, account session keys, drafts, secret chats, settings.

* Back up "tdata" regularly. It contains your account session and your
  secret chats: deleting it logs you out and destroys secret chats
  forever (they are stored only on this device).
* Do not keep this folder inside OneDrive / Dropbox / other sync
  folders: tdata is written constantly and cloud sync corrupts it -
  and would upload your chat data to the cloud.
* If the folder is not writable (for example you extracted into
  Program Files), the app silently switches to %APPDATA%\ekzegram
  instead. Nothing is lost - your data is simply in the other place.

Migrating between the portable and installer flavors
----------------------------------------------------
* portable -> installer: quit ekzegram, run ekzegram-setup.exe, then
  move "tdata" to %APPDATA%\ekzegram\tdata.
* installer -> portable: quit ekzegram, move %APPDATA%\ekzegram\tdata
  next to ekzegram.exe as "tdata".

The installer flavor (ekzegram-setup.exe) always keeps data in
%APPDATA%\ekzegram and removes it on uninstall.

More information: https://github.com/nyakze/ekzegram#readme
