#pragma once

#include <functional>
#include <string>
#include <map>
#include "steps_common.hpp"
#include "steps_normal.hpp"

using StepsProvider = std::function<std::vector<Step>()>;

namespace core::steps {

    std::map<std::string, StepsProvider> modes_registry = {
        {"Normal", core::steps::get_normal_installation_steps}
    };

}
