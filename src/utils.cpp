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
#include <ctime>
#include <iostream>

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
    std::time_t now = std::time(nullptr);
    std::string time_buffer(100, '\0');
    if (std::strftime(&time_buffer[0], time_buffer.size(),
                      "%Y%m%d-%H%M%S", std::localtime(&now)))
        return time_buffer;
    std::cerr << "ERROR: Failed to fetch current timestamp."
                " Is your system clock service running?\n";
    return "FAILED_TO_GET_TIME";
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
