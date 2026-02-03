#include "include/utils.h"
#include "include/bktool.h"
#include "include/prjcli.h"

#include <iostream>
#include <cstdlib>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "ERROR: No arguments provided.\n\n";
        show_help(argv[0]);
        return 1;
    } else if (argc > 3) {
        std::cerr << "ERROR: Too many arguments.\n\n";
        show_help(argv[0]);
        return 1;
    }
    if (std::string(argv[1]) == "help") {
        show_help(argv[0]);
        return 0;
    }
    if (!is_root()) {
        std::cerr << "ERROR: Access denied - are you running this program as sudo or root?\n";
        std::cerr << "Please consult your administrator if you are unsure.\n";
        exit(1);
    }
    if (logbk_cli(argc, argv)) {
        std::cerr << "Execution terminated with errors.\n";
    } else {
        std::cout << "Execution successfully completed. No errors found.\n";
    }
    return 0;
}
