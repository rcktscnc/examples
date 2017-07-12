#ifndef _TOOL_PROCUTIL_H
#define _TOOL_PROCUTIL_H

#include <tool/types.h>
#include <tool/error_code.h>

namespace tool
{

// Overload: Assumes the current process is the target process.
dword_t process_id(error_code* err = nullptr) noexcept;
// Overload: The name of the target process is passed explicitly.
dword_t process_id(const char* process_name,
	error_code* err = nullptr) noexcept;

// Overload: The ID of the target process is passed explicitly.
void* module_address(dword_t proc_id,
	const char* module_name, error_code* err = nullptr) noexcept;
// Overload: Assumes the current process is the target process.
void* module_address(
	const char* module_name, error_code* err = nullptr) noexcept;
// Overload: The name of the target process is passed explicitly.
void* module_address(const char* process_name,
	const char* module_name, error_code* err = nullptr) noexcept;

// Overload: The ID of the target process is passed explicitly.
error_code read_memory(dword_t proc_id,
	void* address, void* buffer, size_t size) noexcept;
// Overload: Assumes the current process is the target process.
error_code read_memory(
	void* address, void* buffer, size_t size) noexcept;
// Overload: The name of the target process is passed explicitly.
error_code read_memory(const char* process_name,
	void* address, void* buffer, size_t size) noexcept;

// Overload: The ID of the target process is passed explicitly.
error_code write_memory(dword_t proc_id,
	void* address, void* buffer, size_t size) noexcept;
// Overload: Assumes the current process is the target process.
error_code write_memory(
	void* address, void* buffer, size_t size) noexcept;
// Overload: The name of the target process is passed explicitly.
error_code write_memory(const char* process_name,
	void* address, void* buffer, size_t size) noexcept;

//
error_code inject_dll(const char* process_name,
	const char* dll_path) noexcept;

} // namespace

#endif