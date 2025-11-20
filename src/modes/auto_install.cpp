#include "auto_install.hpp"

int AutoInstall::run(WINDOW* win) {
        StepManager manager;

        manager.add(std::make_unique<ShowPartitionInfo>());
        manager.add(std::make_unique<ShowDisks>());

        manager.run(win);
        return 0;
    }