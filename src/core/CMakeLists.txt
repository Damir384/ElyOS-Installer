add_library(core STATIC 
    core.cpp 
    args.cpp
    env.cpp
    welcome.cpp
    state_machine.cpp
    steps/steps_common.cpp
    steps/steps_normal.cpp
)

target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)

target_link_libraries(core PUBLIC 
    ui 
    utils
)