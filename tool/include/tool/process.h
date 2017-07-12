#ifndef _TOOL_PROCESS_H
#define _TOOL_PROCESS_H

#include <tool/types.h>
#include <tool/error_code.h>

namespace tool
{

class process
{
public:
	// You shall not copy.
	process(const process&) = delete;
	// You shall not assign.
	process& operator=(const process&) = delete;
	
	// Constructor will throw if it can't resolve the ID of the process.
	// This version assumes the current process is the target process.
	process() noexcept(false);
	// Constructor will throw if it can't resolve the name of the process.
	process(const char* process_name) noexcept(false);
	// Constructor will throw if it can't resolve the ID of the process.
	process(dword_t proc_id) noexcept(false);
	~process() noexcept;

	//
	dword_t id() const noexcept;

	// Returns the base address of a module in the process.
	void* module_address(const char* module_name, 
		error_code* err = nullptr) noexcept;
	
	// Reads memory relative to the base address of the process.
	error_code read_memory(void* address, void* buffer, size_t size) noexcept;
	// Reads memory relative to the base address of a module.
	error_code read_module_memory(const char* module_name, void* offset,
		void* buffer, size_t size) noexcept;

	// Writes to memory relative to the base address of the process.
	error_code write_memory(void* address, void* buffer, size_t size) noexcept;
	// Writes to memory relative to the base address of a module.
	error_code write_module_memory(const char* module_name, void* offset,
		void* buffer, size_t size) noexcept;

	//
	error_code inject_dll(const char* dll_path) noexcept;
	
private:
	dword_t _process_id;
	void* _process_handle;
	
	// Code in this function is shared by constructors.
	void _init_handle() noexcept(false);
};

} // namespace

#endif