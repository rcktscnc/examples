#ifndef _TOOL_SOCKET_H
#define _TOOL_SOCKET_H

#include <tool/error_code.h>
#include <tool/types.h>
#include <tool/buffer.h>

namespace tool
{

class socket
{
	friend class acceptor;
	friend class connector;
public:
	socket() = default;
	socket(const socket&) = delete;
	socket& operator= (socket&) = delete;
	socket(socket&&) = delete;
	socket& operator= (socket&&) = delete;

	~socket() noexcept;
	
	int read(byte_t* buf, size_t len, error_code* err = nullptr) noexcept;
	int read(buffer* buf, error_code* err = nullptr) noexcept;
	template<typename T>
	int read(T* buf, error_code* err = nullptr) noexcept;

	int write(byte_t* buffer, size_t len, error_code* err = nullptr) noexcept;
	int write(buffer* buf, error_code* err = nullptr) noexcept;
	template<typename T>
	int write(T* buf, error_code* err = nullptr) noexcept;

	error_code remote_address(char* buf, size_t len) noexcept;
	int remote_port() noexcept;

	error_code close() noexcept;

private:
	socket_t _comm_socket;
};

template<typename T>
int socket::write(T* buf, error_code* err) noexcept
{
	return write((byte_t*)buf, sizeof(T), err);
}

template<typename T>
int socket::read(T* buf, error_code* err) noexcept
{
	return read((byte_t*)buf, sizeof(T), err);
}

} // namespace

#endif