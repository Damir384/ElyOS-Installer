function(get_latest_git_tag repo_url out_var)
    execute_process(
        COMMAND git ls-remote --tags --refs --sort=v:refname "${repo_url}"
        OUTPUT_VARIABLE raw_tags
        RESULT_VARIABLE res
        TIMEOUT 5
    )
    if(res EQUAL 0 AND raw_tags)
        string(STRIP "${raw_tags}" raw_tags)
        string(REGEX MATCH "[^\n]+$" last_line "${raw_tags}")
        string(REGEX REPLACE ".*/" "" tag "${last_line}")
        set(${out_var} "${tag}" PARENT_SCOPE)
    else()
        message(WARNING "Failed to check latest version for ${repo_url} (res=${res})")
    endif()
endfunction()

macro(manage_dependency NAME REPO TAG)
    message(STATUS "Checking dependency: ${NAME}...")

    set(_NEEDS_FETCH TRUE)
    find_package(${NAME} QUIET)

    if(${NAME}_FOUND OR TARGET ${NAME} OR TARGET ${NAME}::${NAME})
        message(STATUS "  [System] Found ${NAME}")
        if(NOT ELY_UPDATE_POLICY STREQUAL "UPDATE")
            set(_NEEDS_FETCH FALSE)
        endif()
    endif()

    if(_NEEDS_FETCH)
        set(_TARGET_TAG "${TAG}")
        unset(_LATEST_TAG)

        if(ELY_UPDATE_POLICY STREQUAL "UPDATE")
            get_latest_git_tag("${REPO}" _LATEST_TAG)
            if(_LATEST_TAG)
                set(_TARGET_TAG "${_LATEST_TAG}")
                message(STATUS "  [Update] Latest tag for ${NAME}: ${_TARGET_TAG}")
            endif()
        elseif(EXISTS "${CMAKE_SOURCE_DIR}/libs/${NAME}/CMakeLists.txt")
            message(STATUS "  [Local] Using cached ${NAME} in libs/${NAME}")
            string(TOUPPER "${NAME}" _NAME_UPPER)
            set(FETCHCONTENT_SOURCE_DIR_${_NAME_UPPER} "${CMAKE_SOURCE_DIR}/libs/${NAME}")
        endif()

        FetchContent_Declare(
            ${NAME}
            GIT_REPOSITORY "${REPO}"
            GIT_TAG        "${_TARGET_TAG}"
            GIT_SHALLOW    TRUE
            SOURCE_DIR     "${CMAKE_SOURCE_DIR}/libs/${NAME}"
        )
        FetchContent_MakeAvailable(${NAME})
    endif()
endmacro()

# FTXUI
manage_dependency(ftxui "https://github.com/ArthurSonzogni/FTXUI" "v7.0.3")

# nlohmann_json
manage_dependency(nlohmann_json "https://github.com/nlohmann/json" "v3.12.0")
