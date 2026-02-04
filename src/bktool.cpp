/*
logbk - A simple C++ log backup program
Copyright (C) 2026  NakamuraSama072

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "../include/bktool.h"
#include "../include/utils.h"

#include <string>
#include <iostream>
#include <fstream>
#include <cstdio>

const std::string LOG_BACKUP_DIR = "/root/syslogbackup";
const mode_t DEFAULT_MODE = 0755;

bool log_cleanup(const std::string &path_to_log) {
    std::ofstream log_ostream(path_to_log, std::ios::trunc);
    if (!log_ostream.is_open()) {
        std::cerr << "ERROR: Unable to open log file " 
                << path_to_log << " for cleaning.\n";
        return 1;
    }
    log_ostream.close();
    return 0;
}

bool create_directory(const std::string &dir_path, mode_t mode = DEFAULT_MODE) {
    if (file_or_dir_exists(dir_path)) {
        if (!is_directory(dir_path)) {
            std::cerr << "ERROR: A file with the same name as the desired "
                      << "directory already exists: " << dir_path << "\n";
            return false;
        }
        return true;
    }
    if (mkdir(dir_path.c_str(), mode) != 0) {
        std::cerr << "ERROR: Failed to create directory: " << dir_path << "\n";
        return false;
    }
    return true;
}

bool archive_logs(const std::string &path_to_log,
                  const std::string &custom_backup_dir) {
    struct stat file_stat;
    if (stat(path_to_log.c_str(), &file_stat) != 0) {
        std::cerr << "ERROR: Failed to retrieve information regarding " 
                  << path_to_log << ".\n";
        return 1;
    }
    std::cout << "Log file " << path_to_log << " detected.\n";
    if (!S_ISREG(file_stat.st_mode)) {
        std::cerr << "ERROR: " << path_to_log << " is not a regular file.\n";
        return 1;
    }
    unsigned long file_size = file_stat.st_size;
    std::string readable_size = format_file_size(file_size);
    std::cout << "Size: " << readable_size << "\n";
    if (file_size == 0) {
        std::cout << "The log file appears to be empty. Backup is not necessary.\n";
        return 0;
    }
    std::cout << "Attempting to create/verify backup directory...\n";
    if (!create_directory(custom_backup_dir)) {
        std::cerr << "ERROR: Failed to create or verify backup directory.\n";
        return 1;
    }
    size_t last_delimiter = path_to_log.find_last_of('/');
    std::string file_name = (last_delimiter != std::string::npos) ?
                            path_to_log.substr(last_delimiter + 1) : path_to_log;
    std::string file_dir = (last_delimiter != std::string::npos) ?
                            path_to_log.substr(0, last_delimiter) : ".";
    std::string timestamp = get_current_timestamp();
    std::string backup_filename = custom_backup_dir + "/"
                                  + file_name + "_" + timestamp + ".tar.xz";
    std::cout << "Attempting to create archives...\n";
    if (!create_tar_archive(file_dir, file_name, backup_filename)) {
        std::cerr << "ERROR: Failed to create archive.\n";
        if (file_or_dir_exists(backup_filename))
            remove(backup_filename.c_str());
        return 1;
    }
    std::cout << "Verifying if archive is created...\n";
    if (!file_or_dir_exists(backup_filename)) {
        std::cerr << "ERROR: Archive file verification failed.\n";
        return 1;
    }
    std::cout << "Archive created successfully at " << backup_filename << "\n";
    std::cout << "Cleaning original log file...\n";
    if (log_cleanup(path_to_log)) {
        std::cerr << "ERROR: Failed to clean log file, however archive was created successfully.\n";
        return 1;
    }
    std::cout << "Job completed successfully.\n";
    return 0;
}
