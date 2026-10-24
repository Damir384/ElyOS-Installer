#include "welcome.hpp"
#include "utils/console.hpp"

namespace core {

void print_welcome_message() {
    utils::print_console(
        "------------------------------------------------------------------\n"
        " ElyOS Installer [Arch Linux powered] _\n"
        " Autonomy is not isolation, but independence. _\n"
        " Probing hardware... Automate what you want, control what you need.\n"
        "------------------------------------------------------------------",
        /*log_to_file=*/ false
    );
}

}