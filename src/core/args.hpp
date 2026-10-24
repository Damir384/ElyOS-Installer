#pragma once

#include <string>
#include <vector>

struct ParsedArgs {
    bool debug = false;
    bool show_help = false;
    std::vector<std::string> unknown_args;
};

namespace core {
    ParsedArgs parse_arguments(int argc, char* argv[]);
}
