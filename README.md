# loadavgmonitor

Requires sudo or root privileges by default since it writes to `/var/lib/loadavgmonitord/main.sqlite".

Flags:
* `-f` - database file to write to
* `-d` - delay between measurements in milliseconds

This program monitors your Linux computer's load average and logs it into an
SQLite database.
