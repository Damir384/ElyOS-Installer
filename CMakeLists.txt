cmake_minimum_required(VERSION 3.20)

project(ElyOS_Installer VERSION 0.1.0 LANGUAGES CXX)

add_compile_definitions(
    ELY_APP_NAME="ElyOS Installer"
    ELY_VERSION="${PROJECT_VERSION}"
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(FETCHCONTENT_BASE_DIR "${CMAKE_SOURCE_DIR}/libs")

set(ELY_UPDATE_POLICY "ASK" CACHE STRING "Action when library is outdated: IGNORE, UPDATE, FAIL")
set_property(CACHE ELY_UPDATE_POLICY PROPERTY STRINGS "IGNORE" "UPDATE" "FAIL")

include(FetchContent)
include(cmake/dependencies.cmake)

add_subdirectory(src)
