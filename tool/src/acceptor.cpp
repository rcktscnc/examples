#include <tool/def.h>
#include <tool/acceptor.h>
#include <tool/exception.h>
#include <Winsock2.h>
#include <Ws2tcpip.h>

namespace tool
{

class acceptor::acc_impl
{
public:
	acc_impl(const char* port) noexcept(false);
	~acc_impl() noexcept;

	error_code accept(socket* sckt) noexcept;

private:
	SOCKET _listen_socket;
	addrinfo* _result;
	addrinfo _hints;

	error_code _init_socket(const char* port) noexcept;
};

acceptor::acc_impl::acc_impl(const char* port) noexcept(false)
{
	_listen_socket = INVALID_SOCKET;
	_result = NULL;

	ZeroMemory(&_hints, sizeof(_hints));
	_hints.ai_family = AF_INET;
	_hints.ai_socktype = SOCK_STREAM;
	_hints.ai_protocol = IPPROTO_TCP;
	_hints.ai_flags = AI_PASSIVE;

	error_code err = _init_socket(port);
	if (err != error_code::NO_ERR) throw tool::exception(err);
}

acceptor::acc_impl::~acc_impl() noexcept
{
	closesocket(_listen_socket);
	freeaddrinfo(_result);
}

error_code acceptor::acc_impl::_init_socket(const char* port) noexcept
{
	// Resolve the server address and port
	int win_err = getaddrinfo(nullptr, port, &_hints, &_result);
	if (win_err != 0) return error_code::CANNOT_GET_ADDRESS_INFO;

	// Create a SOCKET for connecting to server
	_listen_socket = ::socket(_result->ai_family, _result->ai_socktype,
		_result->ai_protocol);
	if (_listen_socket == INVALID_SOCKET)
		return error_code::CANNOT_CREATE_SOCKET;

	// Setup the TCP listening socket
	win_err = bind(_listen_socket, _result->ai_addr, (int)_result->ai_addrlen);
	if (win_err == SOCKET_ERROR) return error_code::CANNOT_BIND_SOCKET;

	win_err = ::listen(_listen_socket, SOMAXCONN);
	if (win_err == SOCKET_ERROR) return error_code::CANNOT_LISTEN_TO_SOCKET;

	return error_code::NO_ERR;
}

error_code acceptor::acc_impl::accept(socket* sckt) noexcept
{
	SOCKET comm_socket = ::accept(_listen_socket, NULL, NULL);
	if (comm_socket == INVALID_SOCKET) return error_code::CANNOT_ACCEPT_CONNECTION;

	sckt->_comm_socket = comm_socket;
	return error_code::NO_ERR;
}

// Acceptor Interface

acceptor::acceptor(const char* port) noexcept(false)
	: _acc_pimpl(new acc_impl(port))
{
}

acceptor::acceptor(acceptor&& rhs) noexcept
{
	_acc_pimpl = rhs._acc_pimpl;
	rhs._acc_pimpl = nullptr;
}

acceptor& acceptor::operator= (acceptor&& rhs) noexcept
{
	_acc_pimpl = rhs._acc_pimpl;
	rhs._acc_pimpl = nullptr;

	return *this;
}

acceptor::~acceptor() noexcept
{
	delete _acc_pimpl;
}

error_code acceptor::accept(socket* sckt) noexcept
{
	return _acc_pimpl->accept(sckt);
}

} // namespace