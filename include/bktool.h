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

#ifndef BKTOOL_H
#define BKTOOL_H

#include <string>
#include <sys/stat.h>

bool log_cleanup(const std::string &path_to_log);
bool create_directory(const std::string &dir_path, mode_t mode);
bool archive_logs(const std::string &path_to_log, const std::string &custom_backup_dir);

#endif
