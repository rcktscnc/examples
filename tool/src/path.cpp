#include <Windows.h>
#include <io.h> // _access()
#include <tool/path.h>

namespace tool
{

bool path_exists(const char* file_path, error_code* err) noexcept
{
	if (err) *err = error_code::NO_ERR;

	if (_access(file_path, 0))
	{
		if (err) *err = error_code::INVALID_PATH;
		return false;
	}

	return true;
}

error_code full_path_to(const char* path, char* buffer) noexcept
{
	if (!GetFullPathNameA(path, PATH_SIZE, buffer, NULL))
		return error_code::CANNOT_GET_FULL_PATH;

	return error_code::NO_ERR;
}

error_code current_path(char* buffer) noexcept
{
	if (!GetCurrentDirectoryA(PATH_SIZE, buffer))
		return error_code::CANNOT_GET_CURRENT_DIRECTORY;

	return error_code::NO_ERR;
}

} // namespace