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
        std::cerr << "logbk  Copyright (C) 2026  NakamuraSama072\n";
        std::cerr << "This program comes with ABSOLUTELY NO WARRANTY; for details type `show w'.\n";
        std::cerr << "This is free software, and you are welcome to redistribute it\n";
        std::cerr << "under certain conditions; type `show c' for details.\n";
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
