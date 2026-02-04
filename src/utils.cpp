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

#include "../include/utils.h"

#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <ctime>
#include <iostream>
#include <chrono>
#include <time.h>
#include <sstream>

bool is_root() {
    return geteuid() == 0;
}

bool file_or_dir_exists(const std::string &path) {
    return access(path.c_str(), F_OK) == 0;
}

bool is_directory(const std::string &path) {
    struct stat path_stat;
    if (stat(path.c_str(), &path_stat) != 0) return false;
    return S_ISDIR(path_stat.st_mode);
}

bool is_regular_file(const std::string &path) {
    struct stat path_stat;
    if (stat(path.c_str(), &path_stat) != 0) return false;
    return S_ISREG(path_stat.st_mode);
}

std::string get_current_timestamp() {
    auto now = std::chrono::system_clock::now();
    if (!now.time_since_epoch().count()) {
        std::cerr << "WARNING: Failed to get current system time." << 
                     " Is your system clock service running?\n";
        return "NO_TIME_AVAILABLE";
    }
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);

    std::tm local_time;
    if (localtime_r(&now_time, &local_time) == nullptr) {
        std::cerr << "WARNING: Failed to convert time to local time.\n";
        return "CONVERSION_FAILED";
    }

    std::ostringstream oss;
    oss << std::put_time(&local_time, "%Y%m%d-%H%M%S");
    return oss.str();
}

std::string format_file_size(unsigned long size_in_bytes) {
    const std::string size_units[] = {"B", "KiB", "MiB", "GiB", "TiB", "PiB"};
    unsigned short unit_index = 0;
    double formatted_size = static_cast<double>(size_in_bytes);
    while (formatted_size >= 1024 && unit_index < 5) {
        unit_index++;
        formatted_size = formatted_size / 1024;
    }
    formatted_size = static_cast<unsigned long>
                    (formatted_size * 100 + 0.5) / 100.0;
    std::string readable_size = std::to_string(formatted_size) +
                                " " + size_units[unit_index];
    return readable_size;
}

bool create_tar_archive(const std::string &source_path, 
                        const std::string &file_name, 
                        const std::string &archive_path) {
    pid_t pid = fork();
    if (pid < 0) {
        std::cerr << "ERROR: Failed to fork process.\n";
        return false;
    }
    if (pid == 0) {
        execlp("tar", "tar", "-cJf", archive_path.c_str(), 
               "-C", source_path.c_str(), file_name.c_str(), nullptr);
        _exit(EXIT_FAILURE);
    }
    int status;
    waitpid(pid, &status, 0);
    return WIFEXITED(status) && (WEXITSTATUS(status) == 0);
}
