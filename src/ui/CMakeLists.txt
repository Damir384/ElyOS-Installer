add_library(ui STATIC 
    ftxui_context.cpp
)

target_include_directories(ui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)

# Привязка FTXUI через мета-таргет
target_link_libraries(ui PUBLIC 
    ftxui::ftxui
)