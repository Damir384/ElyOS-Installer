#pragma once
#include <string>

namespace utils {
    /**
     * @brief Checks if a file physically exists on the filesystem.
     * @param [in] path Absolute or relative path to check.
     * @param [out] message Output string populated with diagnostic details if check fails.
     * @return int Returns 0 if file exists, -1 otherwise.
     */
    int check_file_exist(const std::string& path, std::string& message);

    /**
     * @brief Verifies read permissions for a given file path using access(R_OK).
     * @param [in] path File path to test.
     * @param [out] message Output string containing system errno details upon failure.
     * @return int Returns 0 if file is readable, -1 otherwise.
     */
    int check_file_readable(const std::string& path, std::string& message);

    /**
     * @brief Reads the entire contents of a file into a single string.
     * @param [in] path File path to open.
     * @return std::string File content buffer, or empty string on failure.
     */
    std::string read_file(const std::string& path);
}