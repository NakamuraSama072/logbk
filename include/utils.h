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
