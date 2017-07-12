#include <tool/def.h>
#include <tool/process.h>
#include <tool/procutil.h>
#include <tool/exception.h>
#include <tool/path.h>
#include <Windows.h>

namespace tool
{

process::process() noexcept(false) : process(process_id())
{
}

process::process(const char* process_name) noexcept(false)
{
	error_code err;
	_process_id = process_id(process_name, &err);
	if (err != error_code::NO_ERR) throw tool::exception(err);
	
	_init_handle();
}

process::process(dword_t proc_id) noexcept(false)
{
	_process_id = proc_id;
	_init_handle();
}

process::~process() noexcept
{
	CloseHandle(_process_handle);
}

void process::_init_handle() noexcept(false)
{
	_process_handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, _process_id);
	if (!_process_handle)
		throw tool::exception(error_code::CANNOT_OPEN_PROCESS);
}

dword_t process::id() const noexcept { return _process_id; }

void* process::module_address(const char* module_name, 
	error_code* err) noexcept
{
	return tool::module_address(_process_id, module_name, err);
}

error_code process::read_memory(void* address, void* buffer,
	size_t size) noexcept
{
	if (!ReadProcessMemory(_process_handle, address, buffer, size, NULL))
		return error_code::CANNOT_READ_FROM_PROCESS;
	
	return error_code::NO_ERR;
}

error_code process::read_module_memory(const char* module_name, void* offset,
	void* buffer, size_t size) noexcept
{
	error_code err;
	void* module_addr = process::module_address(module_name, &err);
	if (err != error_code::NO_ERR) return err;

	return read_memory((void*)((dword_t)module_addr + (dword_t)offset),
		buffer, size);
}

error_code process::write_memory(void* address, void* buffer,
	size_t size) noexcept
{
	if(!WriteProcessMemory(_process_handle, address, buffer, size, NULL))
		return error_code::CANNOT_WRITE_TO_PROCESS;

	return error_code::NO_ERR;
}

error_code process::write_module_memory(const char* module_name, void* offset,
	void* buffer, size_t size) noexcept
{
	error_code err;
	void* module_addr = process::module_address(module_name, &err);
	if (err != error_code::NO_ERR) return err;

	return write_memory((void*)((dword_t)module_addr + (dword_t)offset),
		buffer, size);
}

error_code process::inject_dll(const char* dll_path) noexcept
{
	char full_dll_path[PATH_SIZE] = { 0 };

	error_code err = full_path_to(dll_path, full_dll_path);
	if (err != error_code::NO_ERR) return err;

	if (!path_exists(full_dll_path, &err))
		return err;

	// Allocate memory in the remote process for our string argument.
	LPVOID remote_argument = VirtualAllocEx(_process_handle, NULL,
		strlen(full_dll_path), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if (!remote_argument)
		return error_code::CANNOT_ALLOCATE_MEMORY;

	// Write the string in the allocated memory.
	err = write_memory(remote_argument, (void*)full_dll_path,
		strlen(full_dll_path));
	if (err != error_code::NO_ERR) return err;

	// As an alternative, you can use GetModuleHandleA()
	void* module_addr = module_address("kernel32.dll", &err);
	if (err != error_code::NO_ERR) return err;

	// Get address of the function LoadLibrary, which is in the module Kernel32.dll
	LPVOID load_library =
		(LPVOID)GetProcAddress((HMODULE)module_addr, "LoadLibraryA");
	if (!load_library)
		return error_code::CANNOT_GET_FUNCTION_ADDRESS;

	// Call LoadLibraryA remotely and pass our string (DLL path) as argument to it.
	// The function LoadLibraryA has the same address in every Windows process.
	// This is how we know it will have the same address on the target process.
	if (!CreateRemoteThread(_process_handle, NULL, 0,
		(LPTHREAD_START_ROUTINE)load_library, remote_argument, 0, NULL))
	{
		return error_code::CANNOT_CREATE_REMOTE_THREAD;
	}
	
	return error_code::NO_ERR;
}

} // namespace