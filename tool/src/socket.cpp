#include <tool/def.h>
#include <tool/socket.h>
#include <Winsock2.h>
#include <Ws2tcpip.h>

namespace tool
{

socket::~socket() noexcept
{
	closesocket(_comm_socket);
}

int socket::read(byte_t* buf, size_t len, error_code* err) noexcept
{
	if (err) *err = error_code::NO_ERR;

	int bytes_read = recv(_comm_socket, (char*)buf, len, 0);
	if (err && bytes_read == SOCKET_ERROR)
		*err = error_code::CANNOT_READ_SOCKET;
	
	return bytes_read;
}

int socket::read(buffer* buf, error_code* err) noexcept
{
	// Reads first [sizeof(size_t)] bytes of the message, which is the size of the message;
	int bytes_read = read(buf->_buf, sizeof(size_t), err);
	if (buf->size() + sizeof(size_t) > buf->_capacity)
	{
		if (err) *err = error_code::BUFFER_OUT_OF_RANGE;
		return bytes_read;
	}
	
	return bytes_read + read(buf->data(), buf->size(), err);
}

int socket::write(byte_t* buf, size_t len, error_code* err) noexcept
{
	if (err) *err = error_code::NO_ERR;

	int bytes_sent = send(_comm_socket, (char*)buf, len, 0);
	if (err && bytes_sent == SOCKET_ERROR)
		*err = error_code::CANNOT_WRITE_SOCKET;

	return bytes_sent;
}

int socket::write(buffer* buf, error_code* err) noexcept
{
	return write(buf->_buf, buf->size() + sizeof(size_t), err);
}

#ifdef __MINGW32__
const char* _inet_ntop(int af, const void* src, char* dst, int cnt)
{
	struct sockaddr_in srcaddr;
	memset(&srcaddr, 0, sizeof(struct sockaddr_in));
	memcpy(&(srcaddr.sin_addr), src, sizeof(srcaddr.sin_addr));

	srcaddr.sin_family = af;
	if (WSAAddressToStringA((struct sockaddr*) &srcaddr,
		sizeof(struct sockaddr_in), 0, (LPSTR)dst, (LPDWORD)&cnt) != 0)
	{
		return NULL;
	}

	return dst;
}
#endif

error_code socket::remote_address(char* buf, size_t len) noexcept
{
	sockaddr_in remote_info = { 0 };
	int remote_size = sizeof(remote_info);
	getpeername(_comm_socket, (sockaddr*)&remote_info, &remote_size);
#ifdef __MINGW32__
	if (!_inet_ntop(remote_info.sin_family, &remote_info.sin_addr, buf, len))
#else
	if (!inet_ntop(remote_info.sin_family, &remote_info.sin_addr, buf, len))
#endif
		return error_code::CANNOT_GET_ADDRESS_INFO;
	
	return error_code::NO_ERR;
}

int socket::remote_port() noexcept
{
	sockaddr_in remote_info = { 0 };
	int remote_size = sizeof(remote_info);
	getpeername(_comm_socket, (sockaddr*)&remote_info, &remote_size);

	return ntohs(remote_info.sin_port);
}

error_code socket::close() noexcept
{
	int win_err = closesocket(_comm_socket);
	if (win_err != 0) return error_code::CANNOT_CLOSE_SOCKET;

	return error_code::NO_ERR;
}

} // namespace