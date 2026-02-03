# Read before Proceeding
This README.MD is still working in progress.

# logbk
A simple Linux log archiving CLI program written in C++

# Requirements
- OS: Linux (6.x recommended)
- Compiler: GCC 6.1 or later / Clang 3.4 or later
- Standard: C++ 17
- Build System: CMake 3.10 or later

# Build and Run
```sh
mkdir build
cd build
cmake ..
cmake --build .
sudo ./logbk
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
```sh
logbk /var/log/syslog
logbk /var/log/auth.log /root/mybackups
logbk help
```

Notes:
- Backups are compressed with xz compression (.tar.xz)
- Original log files are truncated after backup
- Backup filenames include timestamp (YYYYMMDD-HH24MMSS)

