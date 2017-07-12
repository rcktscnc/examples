#include <tool/def.h>
#include <tool/net.h>
#include <Winsock2.h>
#include <Ws2tcpip.h>

namespace tool
{

error_code net_start()
{
	WSADATA _wsa_data;
	int win_err = WSAStartup(MAKEWORD(2, 2), &_wsa_data);
	if (win_err != 0) return error_code::WSASTARTUP_FAILED;

	return error_code::NO_ERR;
}

error_code net_stop()
{
	int win_err = WSACleanup();
	if (win_err != 0) return error_code::WSACLEANUP_FAILED;

	return error_code::NO_ERR;
}

// Friends with tool::socket so we can set the value of the socket.
class connector
{
public:
	connector(socket* sckt) : _sckt(sckt)
	{
	}
	void set_comm_socket(SOCKET _socket)
	{
		_sckt->_comm_socket = _socket;
	}

private:
	socket* const _sckt;
};

error_code connect(socket* sckt, const char* address, const char* port)
{
	SOCKET _comm_socket = INVALID_SOCKET;
	addrinfo* _result = NULL;
	addrinfo _hints;

	ZeroMemory(&_hints, sizeof(_hints));
	_hints.ai_family = AF_INET;
	_hints.ai_socktype = SOCK_STREAM;
	_hints.ai_protocol = IPPROTO_TCP;
	_hints.ai_flags = AI_PASSIVE;

	int win_err = getaddrinfo(address, port, &_hints, &_result);
	if (win_err != 0) return error_code::CANNOT_GET_ADDRESS_INFO;

	for (addrinfo* ptr = _result; ptr != NULL; ptr = ptr->ai_next)
	{
		_comm_socket = ::socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
		if (_comm_socket == INVALID_SOCKET)
		{
			freeaddrinfo(_result);
			return error_code::CANNOT_CREATE_SOCKET;
		}

		win_err =
			::connect(_comm_socket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (win_err == SOCKET_ERROR)
		{
			_comm_socket = INVALID_SOCKET;
			continue;
		}

		break;
	}

	freeaddrinfo(_result);
	if (_comm_socket == INVALID_SOCKET) return error_code::CANNOT_CONNECT_TO_SERVER;

	connector connector(sckt);
	connector.set_comm_socket(_comm_socket);

	return error_code::NO_ERR;
}

} // namespace