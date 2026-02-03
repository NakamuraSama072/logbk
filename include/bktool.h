#ifndef BKTOOL_H
#define BKTOOL_H

#include <string>
#include <sys/stat.h>

bool log_cleanup(const std::string &path_to_log);
std::string safe_execute_command(const std::string &cmd, const std::string &error_msg);
bool create_directory(const std::string &dir_path, mode_t mode);
bool archive_logs(const std::string &path_to_log, const std::string &custom_backup_dir);

#endif
