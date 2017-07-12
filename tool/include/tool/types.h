#ifndef _TOOL_TYPES_H
#define _TOOL_TYPES_H

namespace tool
{
// Same as DWORD
typedef unsigned long dword_t;

// Same as HANDLE
typedef void* handle_t;

// Same as LPVOID
typedef void* lpvoid_t;

// Same as LPCVOID
typedef void* lpcvoid_t;

// Same as SOCKET
typedef unsigned int socket_t;

// Not related to windows.
typedef unsigned int size_t;
typedef unsigned char byte_t;
typedef void* addr_t;

} // namespace

#endif
