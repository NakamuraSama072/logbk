# Read before Proceeding
1. This README.MD is still working in progress.
2. The project is currently under alpha build and has not been fully tested yet, thus errors are inevitable. Please open issues if found one.

# logbk
A simple Linux log archiving CLI program written in C++

[简体中文](./docs/README_zh-cn.md) | [繁體中文](./docs/README_zh-tw.md) | [日本語](./docs/README_ja.md)

# Requirements
- OS: Linux (6.x recommended)
- Compiler: GCC 6.1 or later / Clang 3.4 or later
- Standard: C++ 14
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
Root privilege is requried for running this command.

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

