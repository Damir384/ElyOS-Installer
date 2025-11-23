#pragma once
#include "step.hpp"
#include "dialogs.hpp"
#include "context.hpp"
#include "logger.hpp"
#include <ncurses.h>
#include "json.hpp"
#include <vector>
#include <map>

class ShowPartitionInfo : public Step {
public:
    StepResult run(WINDOW* win) override;
    std::string name() override {
        return "ShowPartitionInfo";
    };
};

class ShowDisks : public Step {
public:
    StepResult run(WINDOW* win) override;
    std::string name() override {
        return "ShowPartitionInfo";
    };
};

class ShowPartitions : public Step {
public:
    StepResult run(WINDOW* win) override;
    std::string name() override {
        return "ShowPartitionInfo";
    };
};
