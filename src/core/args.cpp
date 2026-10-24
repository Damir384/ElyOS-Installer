#include "args.hpp"
#include <string_view>

namespace core {

ParsedArgs parse_arguments(int argc, char* argv[]) {
    ParsedArgs result;

    for (int i = 1; i < argc; ++i) {
        std::string_view arg = argv[i];

        if (arg == "--debug" || arg == "-d") {
            result.debug = true;
        } else if (arg == "--help" || arg == "-h") {
            result.show_help = true;
        } else {
            result.unknown_args.emplace_back(arg);
        }
    }

    return result;
}

} // namespace core
