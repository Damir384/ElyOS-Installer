#include "auto_install.hpp"

int AutoInstall::run(WINDOW* win) {
        StepManager manager;

        manager.selectAddPackages = "selectAddPackages";

        manager.add(std::make_unique<ShowPartitionInfo>());
        manager.add(std::make_unique<ShowDisks>());
        manager.add(std::make_unique<ShowPartitions>());

        manager.run(win);
        return 0;
    }