#ifndef _TOOL_PATH_H
#define _TOOL_PATH_H

#include <tool/error_code.h>

// Same as MAX_PATH
#define PATH_SIZE 260

namespace tool
{

//
bool path_exists(const char* file_path, error_code* err = nullptr) noexcept;

// Does not check whether file or path exists.
error_code full_path_to(const char* path, char* buffer) noexcept;

//
error_code current_path(char* buffer) noexcept;

} // namespace

#endif