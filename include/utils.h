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

#ifndef UTILS_H
#define UTILS_H

#include <string>

bool is_root();
bool file_or_dir_exists(const std::string &path);
bool is_directory(const std::string &path);
bool is_regular_file(const std::string &path);
std::string get_current_timestamp();
std::string format_file_size(unsigned long size_in_bytes);

#endif
