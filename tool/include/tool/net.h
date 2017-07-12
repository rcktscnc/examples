#ifndef _TOOL_NET_H
#define _TOOL_NET_H

#include <tool/def.h>

// Includes <tool/socket.h>, <tool/buffer.h>, <tool/error_code.h> and <tool/types>
#include <tool/acceptor.h>

namespace tool
{

error_code net_start();

error_code net_stop();

error_code connect(socket* sckt, const char* address, const char* port);

} // namespace

#endif