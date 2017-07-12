#include <tool/exception.h>

namespace tool
{

exception::exception(error_code err) noexcept : err(err)
{
}

const char* exception::what() const noexcept
{
	return error_to_string(err);
}

} // namespace