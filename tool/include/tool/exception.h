#ifndef _TOOL_EXCEPTION_H
#define _TOOL_EXCEPTION_H

#include <exception>
#include <tool/error_code.h>

namespace tool
{

class exception : public std::exception
{
public:
	const error_code err;
	exception(error_code err) noexcept;
	virtual const char* what() const noexcept override;
};

} // namespace

#endif