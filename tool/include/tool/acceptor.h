#ifndef _TOOL_ACCEPTOR_H
#define _TOOL_ACCEPTOR_H

#include <tool/error_code.h>
#include <tool/socket.h>

namespace tool
{

class acceptor
{
public:
	acceptor(const acceptor&) = delete;
	acceptor& operator= (const acceptor&) = delete;

	acceptor(const char* port) noexcept(false);
	acceptor(acceptor&& rhs) noexcept;
	acceptor& operator= (acceptor&& rhs) noexcept;
	~acceptor() noexcept;

	error_code accept(socket* sckt) noexcept;

private:
	class acc_impl;
	acc_impl* _acc_pimpl;
};

} // namespace

#endif