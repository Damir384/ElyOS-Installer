function(get_latest_git_tag repo_url out_var)
    execute_process(
        COMMAND git ls-remote --tags --sort=v:refname ${repo_url}
        OUTPUT_VARIABLE raw_tags
        RESULT_VARIABLE res
    )
    if(res EQUAL 0)
        string(REGEX MATCH "[^\n]+$" last_line "${raw_tags}")
        string(REGEX REPLACE ".*/" "" tag "${last_line}")
        set(${out_var} ${tag} PARENT_SCOPE)
    else()
        message(WARNING "Failed to check latest version for ${repo_url}")
    endif()
endfunction()

macro(manage_dependency NAME REPO TAG)
    message(STATUS "Checking dependency: ${NAME}...")

    set(NEEDS_FETCH TRUE)
    get_latest_git_tag(${REPO} LATEST_VERSION)

    find_package(${NAME} QUIET)

    if(${NAME}_FOUND OR TARGET ${NAME} OR TARGET ${NAME}::${NAME})
        message(STATUS "  [System] Found ${NAME}")
        if(NOT ELY_UPDATE_POLICY STREQUAL "UPDATE")
            set(NEEDS_FETCH FALSE)
        endif()
    endif()

    if(NEEDS_FETCH)
        FetchContent_Declare(
            ${NAME}
            GIT_REPOSITORY ${REPO}
            GIT_TAG        ${TAG}
            SOURCE_DIR     "${CMAKE_SOURCE_DIR}/libs/${NAME}"
        )
        FetchContent_MakeAvailable(${NAME})
    endif()
endmacro()

# FTXUI
manage_dependency(ftxui "https://github.com/ArthurSonzogni/FTXUI" "v7.0.3")

# nlohmann_json
manage_dependency(nlohmann_json "https://github.com/nlohmann/json" "v3.12.0")
