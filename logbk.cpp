#include <iostream>
#include <unistd.h>
#include <sys/stat.h>
#include <string>
#include <fstream>
#include <ctime>

// the dir to move log to
const std::string LOG_BACKUP_DIR = "/root/syslogbackup";
// default mode for created directories
const mode_t DEFAULT_MODE = 0755;
// Error message when executing command fails
const std::string CMD_EXEC_ERROR_MSG = "ERROR: Failed to execute command due to popen() failure.\n";

// Check for root privileges
bool is_root() {
    // ID(root) is 0
    return geteuid() == 0;
}

// Check if a file exists
bool file_or_dir_exists(const std::string &path) {
    // No need to use stat here as we just want to check existence
    return access(path.c_str(), F_OK) == 0;
}

// Check if the path is actually a directory
bool is_directory(const std::string &path) {
    struct stat path_stat;
    // We use stat to get the file status
    if (stat(path.c_str(), &path_stat) != 0) return false;
    return S_ISDIR(path_stat.st_mode);
}

// Check if the path is actually a file
bool is_regular_file(const std::string &path) {
    struct stat path_stat;
    if (stat(path.c_str(), &path_stat) != 0) return false;
    return S_ISREG(path_stat.st_mode);
}

// Clean up log file (using ofstream for a safer way)
bool log_cleanup(const std::string &path_to_log);

// Get current timestamp as a string
std::string get_current_timestamp() {
    // get current time
    std::time_t now = std::time(nullptr);

    // time buffer
    std::string time_buffer(100, '\0');
    if (std::strftime(&time_buffer[0], time_buffer.size(),
                      "%Y%m%d-%H%M%S", std::localtime(&now)))
        return time_buffer;
    std::cerr << "ERROR: Failed to fetch current timestamp." <<
                " Is your system clock service running?\n";
    return "FAILED_TO_GET_TIME";
}

// Format file size to human-readable string
std::string format_file_size(unsigned long size_in_bytes) {
    const std::string size_units[] = {"B", "KiB", "MiB", "GiB", "TiB", "PiB"};
    unsigned short unit_index = 0; // only up to PiB (max 5)

    // convert size
    double formatted_size = static_cast<double>(size_in_bytes);
    while (formatted_size >= 1024 && unit_index < 5) {
        unit_index++;
        formatted_size = formatted_size / 1024;
    }
    // keep two decimal places
    formatted_size = static_cast<unsigned long>
                    (formatted_size * 100 + 0.5) / 100.0;

    // construct readable size string
    std::string readable_size = std::to_string(formatted_size) +
                                " " + size_units[unit_index];
    return readable_size;
}

// Execute a command and get the output safely
std::string safe_execute_command(const std::string &cmd,
                                 const std::string &error_msg = CMD_EXEC_ERROR_MSG) {
    std::string result;
    // use popen (process open) to execute command
    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) {
        std::cerr << error_msg;
        return "EXECUTION_FAILED";
    }

    // read the output
    char buffer[128];
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }
    pclose(pipe);
    return result;
}

bool create_directory(const std::string &dir_path, mode_t mode = DEFAULT_MODE) {
    // Check if directory already exists
    if (file_or_dir_exists(dir_path)) {
        if (!is_directory(dir_path)) {
            std::cerr << "ERROR: A file with the same name as the desired "
                      << "directory already exists: " << dir_path << "\n";
            return false; // exists but is a file
        }
        return true; // already exists
    }

    // Create the directory
    if (mkdir(dir_path.c_str(), mode) != 0) {
        std::cerr << "ERROR: Failed to create directory: " << dir_path << "\n";
        return false;
    }
    return true;
}

// Archive and move log files somewhere else
bool archive_logs(const std::string &path_to_log,
                  const std::string &custom_backup_dir = LOG_BACKUP_DIR) {
    struct stat file_stat;

    // Retrieve file info
    if (stat(path_to_log.c_str(), &file_stat) != 0) {
        std::cerr << "ERROR: Failed to retrieve information regarding " 
                  << path_to_log << ".\n";
        return 1;
    }
    std::cout << "Log file " << path_to_log << " detected.\n";

    // Check if it's a regular file
    if (!S_ISREG(file_stat.st_mode)) {
        std::cerr << "ERROR: " << path_to_log << " is not a regular file.\n";
        return 1;
    }

    // File size
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

    // get file name and dir
    size_t last_delimiter = path_to_log.find_last_of('/');
    std::string file_name = (last_delimiter != std::string::npos) ?
                            path_to_log.substr(last_delimiter + 1) : path_to_log;
    std::string file_dir = (last_delimiter != std::string::npos) ?
                            path_to_log.substr(0, last_delimiter) : ".";
    // timestamp and backup file name
    std::string timestamp = get_current_timestamp();
    std::string backup_filename = custom_backup_dir + "/"
                                  + file_name + "_" + timestamp + ".tar.xz";

    std::cout << "Attempting to create archives...\n";
    // archive name
    std::string tar_cmd = "tar -cJf \"" + backup_filename +
                            "\" -C \"" + file_dir 
                            + "\" \"" + file_name + "\"";

    if (safe_execute_command(tar_cmd) == "EXECUTION_FAILED") {
        std::cerr << "ERROR: Failed to create archive.\n";
        // Revert changes
        if (file_or_dir_exists(backup_filename))
            remove(backup_filename.c_str());
        return 1;
    }

    std::cout << "Verifying if archive is created...\n";
    // check if archive exists
    if (!file_or_dir_exists(backup_filename)) {
        std::cerr << "ERROR: Archive file verification failed.\n";
        return 1;
    }
    // Success
    std::cout << "Archive created successfully at " << backup_filename << "\n";

    // Clean original log file
    std::cout << "Cleaning original log file...\n";
    if (log_cleanup(path_to_log)) {
        std::cerr << "ERROR: Failed to clean log file, however archive was created successfully.\n";
        return 1;
    }

    // Complete
    std::cout << "Job completed successfully.\n";
    return 0;
}

// CLI Show Help
void show_help(const std::string& program_name) {
    std::cout << "logbk - A simple C++ log backup program\n";
    std::cout << "Version: alpha build 0.0.5\n";
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

// The main cmd line interface.
bool logbk_cli(int argc, char* argv[]) {
    // Check for help command
    if (argc == 2 && std::string(argv[1]) == "help") {
        show_help(argv[0]);
        return 0;
    }

    // Validate argument count
    if (argc < 2 || argc > 3) {
        std::cerr << "ERROR: Invalid number of arguments.\n\n";
        show_help(argv[0]);
        return 1;
    }

    std::string path_to_log = argv[1];
    std::string backup_dir = (argc == 3) ? argv[2] : LOG_BACKUP_DIR;

    std::cout << "Start processing...\n";

    // Check if the log path is directory
    if (is_directory(path_to_log)) {
        std::cerr << "ERROR: Your log file " << path_to_log
                  << " is actually a directory. Please specify a log file.\n";
        return 1;
    }

    // Check if the log path exists
    if (!file_or_dir_exists(path_to_log)) {
        std::cerr << "ERROR: Log file does not exist.\n";
        return 1;
    }

    // Check if the destination is actually a file
    if (is_regular_file(backup_dir)) {
        std::cerr << "ERROR: Your backup path " << backup_dir
                  << " is actually a file. Please specify a directory.\n";
        return 1;
    }

    // back up the log
    if (archive_logs(path_to_log, backup_dir) != 0) {
        std::cerr << "Found errors when attempting to copy."
                  << " This program will be terminated.\n";
        return 1;
    }

    return 0;
}

int main(int argc, char* argv[]) {
    // Minimum check for arguments
    if (argc < 2) {
        std::cerr << "ERROR: No arguments provided.\n\n";
        show_help(argv[0]);
        return 1;
    } else if (argc > 3) {
        std::cerr << "ERROR: Too many arguments.\n\n";
        show_help(argv[0]);
        return 1;
    }

    // Check for help command (before root check for usability)
    if (std::string(argv[1]) == "help") {
        show_help(argv[0]);
        return 0;
    }

    // Check for root privileges
    if (!is_root()) {
        std::cerr << "ERROR: Access denied - are you running this program as sudo or root?\n";
        std::cerr << "Please consult your administrator if you are unsure.\n";
        exit(1);
    }

    // Run the CLI
    if (logbk_cli(argc, argv)) {
        std::cerr << "Execution terminated with errors.\n";
    } else { // "return_code = 0" means no errors found
        std::cout << "Execution successfully completed. No errors found.\n";
    }
    return 0;
}

// Clean up (using ofstream for a safer way)
bool log_cleanup(const std::string &path_to_log) {
    std::ofstream log_ostream(path_to_log, std::ios::trunc);

    // Check if file is opened successfully
    if (!log_ostream.is_open()) {
        std::cerr << "ERROR: Unable to open log file " << 
                path_to_log << " for cleaning.\n";
        return 1;
    }
    // Directly close the stream after truncation
    // since truncation is done on open
    log_ostream.close();
    return 0;
}