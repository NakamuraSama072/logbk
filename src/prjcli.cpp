#include "../include/prjcli.h"
#include "../include/utils.h"
#include "../include/bktool.h"

#include <iostream>
#include <cstdlib>

extern const std::string LOG_BACKUP_DIR;

void show_help(const std::string& program_name) {
    std::cout << "logbk - A simple C++ log backup program\n";
    std::cout << "Version: alpha build 0.0.6\n";
    std::cout << "Requires root privileges for system log access.\n";
    std::cout << "\nUsage:\n";
    std::cout << "  " << program_name << " <log_file> [backup_directory]\n";
    std::cout << "  " << program_name << " help\n";
    std::cout << "\nArguments:\n";
    std::cout << "  log_file         Path to the log file to backup and clean\n";
    std::cout << "  backup_directory Optional: Custom backup directory (default: " 
              << LOG_BACKUP_DIR << ")\n";
    std::cout << "\nExamples:\n";
    std::cout << "  " << program_name << " /var/log/syslog\n";
    std::cout << "  " << program_name << " /var/log/auth.log /root/mybackups\n";
    std::cout << "  " << program_name << " help\n";
    std::cout << "\nNotes:\n";
    std::cout << "  - Backups are compressed with xz compression (.tar.xz)\n";
    std::cout << "  - Original log files are truncated after backup\n";
    std::cout << "  - Backup filenames include timestamp (YYYYMMDD-HH24MMSS)\n";
}

bool logbk_cli(int argc, char* argv[]) {
    if (argc == 2 && std::string(argv[1]) == "help") {
        show_help(argv[0]);
        return 0;
    }
    if (argc < 2 || argc > 3) {
        std::cerr << "ERROR: Invalid number of arguments.\n\n";
        show_help(argv[0]);
        return 1;
    }
    std::string path_to_log = argv[1];
    std::string backup_dir = (argc == 3) ? argv[2] : LOG_BACKUP_DIR;
    std::cout << "Start processing...\n";
    if (is_directory(path_to_log)) {
        std::cerr << "ERROR: Your log file " << path_to_log
                  << " is actually a directory. Please specify a log file.\n";
        return 1;
    }
    if (!file_or_dir_exists(path_to_log)) {
        std::cerr << "ERROR: Log file does not exist.\n";
        return 1;
    }
    if (is_regular_file(backup_dir)) {
        std::cerr << "ERROR: Your backup path " << backup_dir
                  << " is actually a file. Please specify a directory.\n";
        return 1;
    }
    if (archive_logs(path_to_log, backup_dir) != 0) {
        std::cerr << "Found errors when attempting to copy."
                  << " This program will be terminated.\n";
        return 1;
    }
    return 0;
}
