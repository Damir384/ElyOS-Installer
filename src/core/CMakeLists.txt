add_library(elyos_core STATIC 
    core.cpp 
    env.cpp
    welcome.cpp
    state_machine.cpp
    steps/steps_common.cpp
    steps/steps_normal.cpp
)

target_include_directories(elyos_core PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(elyos_core PUBLIC 
    ui
    utils
)
