add_executable(${PROJECT_NAME} 
    main.cpp
    ftxui_context.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(${PROJECT_NAME} PRIVATE 
    elyos_core 
    ui
    utils
    ftxui::ftxui
)
