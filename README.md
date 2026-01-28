# Read before Proceeding
This README.MD is still working in progress.

# logbk
A simple Linux log archiving CLI program written in C++

# Requirements
OS: Linux (6.x recommended)
Compiler: MinGW g++ 6.1 or later
Standard: C++ 14

# Build and Run
```sh
g++ logbk.cpp -o logbk
chmod +x logbk
./logbk
```

# Usage:
Requires root privileges for system log access.
Usage:
```sh
logbk <log_file> [backup_directory]
logbk help
```
Arguments:

- log_file: Path to the log file to backup and clean
- backup_directory (Optional): Custom backup directory (default: /root/syslogbackup)

Examples:
logbk /var/log/syslog
logbk /var/log/auth.log /root/mybackups
logbk help

Notes:
- Backups are compressed with xz compression (.tar.xz)
- Original log files are truncated after backup
- Backup filenames include timestamp (YYYYMMDD-HH24MMSS)

