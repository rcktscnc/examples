#include <tool/def.h>
#include <tool/procutil.h>
#include <tool/path.h>
#include <Windows.h>
#include <tlhelp32.h>

namespace tool
{

dword_t process_id(error_code* err) noexcept
{
	if (err) *err = error_code::NO_ERR;

	dword_t proc_id = GetCurrentProcessId();
	if (err && !proc_id)
		*err = error_code::CANNOT_GET_PROCESS_ID;
	
	return proc_id;
}

dword_t process_id(const char* process_name, error_code* err) noexcept
{
	if (err) *err = error_code::NO_ERR;
	
	 HANDLE processes_snapshot =
		 CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (processes_snapshot == INVALID_HANDLE_VALUE)
	{
		if (err) *err = error_code::CANNOT_CREATE_PROCESS_SNAPSHOT;
		return 0;
	}

	PROCESSENTRY32 process_entry;
	process_entry.dwSize = sizeof(PROCESSENTRY32);
	BOOL not_empty = Process32First(processes_snapshot, &process_entry);

	while (not_empty)
	{
#if defined(__clang__) || defined(__MINGW32__)
		if (!strcmp(process_entry.szExeFile, process_name))
			return process_entry.th32ProcessID;
#elif defined(_MSC_VER)
		// Convert WCHAR to char*. Only needed if building in Unicode, apparently.
		char exeFileANSI[MAX_PATH] = { 0 };
		WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, process_entry.szExeFile,
			-1, exeFileANSI, sizeof(exeFileANSI), NULL, NULL);

		if (!strcmp(exeFileANSI, process_name))
			return process_entry.th32ProcessID;
#endif
		not_empty = Process32Next(processes_snapshot, &process_entry);
	}

	if (err) *err = error_code::CANNOT_GET_PROCESS_ID;
	return 0;
}

void* module_address(dword_t proc_id, const char* module_name,
	error_code* err) noexcept
{
	if (err) *err = error_code::NO_ERR;

	HANDLE modules_snapshot =
		CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, proc_id);
	if (modules_snapshot == INVALID_HANDLE_VALUE)
	{
		if (err) *err = error_code::CANNOT_CREATE_MODULE_SNAPSHOT;
		return 0;
	}
	
	MODULEENTRY32 module_entry;
	module_entry.dwSize = sizeof(MODULEENTRY32);
	BOOL not_empty = Module32First(modules_snapshot, &module_entry);

	while (not_empty)
	{
#if defined(__clang__) || defined(__MINGW32__)
		if (!strcmp(module_entry.szModule, module_name))
			return module_entry.hModule;
#elif defined(_MSC_VER)
		// Convert WCHAR to char*. Only needed if building in Unicode, apparently.
		char moduleANSI[MAX_PATH] = { 0 };
		WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, module_entry.szModule,
			-1, moduleANSI, sizeof(moduleANSI), NULL, NULL);

		if (!strcmp(moduleANSI, module_name))
			return module_entry.hModule;
#endif
		not_empty = Module32Next(modules_snapshot, &module_entry);
	}

	if (err) *err = error_code::CANNOT_GET_MODULE_ADDRESS;
	return 0;
}

void* module_address(const char* module_name, error_code* err) noexcept
{
	dword_t proc_id = process_id(err);
	if (*err != error_code::NO_ERR) return 0;

	// Call overloaded version.
	return module_address(proc_id, module_name, err);
}

void* module_address(const char* process_name, const char* module_name,
	error_code* err) noexcept
{
	dword_t proc_id = process_id(process_name, err);
	if (*err != error_code::NO_ERR) return 0;
	
	// Call overloaded version.
	return module_address(proc_id, module_name, err);
}

error_code read_memory(dword_t proc_id, void* address, void* buffer,
	size_t size) noexcept
{
	HANDLE process_handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, proc_id);
	if (!process_handle)
		return error_code::CANNOT_OPEN_PROCESS;

	if (!ReadProcessMemory(process_handle, address, buffer, size, NULL))
	{
		CloseHandle(process_handle);
		return error_code::CANNOT_READ_FROM_PROCESS;
	}

	CloseHandle(process_handle);
	return error_code::NO_ERR;
}

error_code read_memory(void* address, void* buffer, size_t size) noexcept
{
	error_code err;
	dword_t proc_id = process_id(&err);
	if (err != error_code::NO_ERR) return err;
	
	// Call overloaded version.
	return read_memory(proc_id, address, buffer, size);
}

error_code read_memory(const char* process_name, void* address,
	void* buffer, size_t size) noexcept
{
	error_code err;
	dword_t proc_id = process_id(process_name, &err);
	if (err != error_code::NO_ERR) return err;

	// Call overloaded version.
	return read_memory(proc_id, address, buffer, size);
}

error_code write_memory(dword_t proc_id, void* address, void* buffer,
	size_t size) noexcept
{
	HANDLE process_handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, proc_id);
	if (!process_handle)
		return error_code::CANNOT_OPEN_PROCESS;

	if (!WriteProcessMemory(process_handle, address, buffer, size, NULL))
	{
		CloseHandle(process_handle);
		return error_code::CANNOT_WRITE_TO_PROCESS;
	}

	CloseHandle(process_handle);
	return error_code::NO_ERR;
}

error_code write_memory(void* address, void* buffer, size_t size) noexcept
{
	error_code err;
	dword_t proc_id = process_id(&err);
	if (err != error_code::NO_ERR) return err;

	// Call overloaded version.
	return write_memory(proc_id, address, buffer, size);
}

error_code write_memory(const char* process_name, void* address,
	void* buffer, size_t size) noexcept
{
	error_code err;
	dword_t proc_id = process_id(process_name, &err);
	if (err != error_code::NO_ERR) return err;

	// Call overloaded version.
	return write_memory(proc_id, address, buffer, size);
}

error_code inject_dll(const char* process_name, const char* dll_path) noexcept
{
	char full_dll_path[PATH_SIZE] = { 0 };

	error_code err = full_path_to(dll_path, full_dll_path);
	if (err != error_code::NO_ERR) return err;

	if (!path_exists(full_dll_path, &err))
		return err;

	dword_t proc_id = process_id(process_name, &err);
	if (err != error_code::NO_ERR) return err;

	HANDLE process_handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, proc_id);
	if (!process_handle)
		return error_code::CANNOT_OPEN_PROCESS;

	// Allocate memory in the remote process for our string argument.
	LPVOID remote_argument = VirtualAllocEx(process_handle, NULL,
		strlen(full_dll_path), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if (!remote_argument)
	{
		CloseHandle(process_handle);
		return error_code::CANNOT_ALLOCATE_MEMORY;
	}

	// Write the string in the allocated memory.
	if (!WriteProcessMemory(process_handle, remote_argument, full_dll_path,
		strlen(full_dll_path), NULL))
	{
		CloseHandle(process_handle);
		return error_code::CANNOT_WRITE_TO_PROCESS;
	}
	
	HMODULE module_handle = GetModuleHandleA("kernel32.dll");
	if (!module_handle)
	{
		CloseHandle(process_handle);
		return error_code::CANNOT_GET_MODULE_ADDRESS;
	}
	
	// The function LoadLibraryA has the same address in every Windows process.
	LPVOID load_library = 
		(LPVOID)GetProcAddress(module_handle, "LoadLibraryA");
	if (!load_library)
	{
		CloseHandle(process_handle);
		return error_code::CANNOT_GET_FUNCTION_ADDRESS;
	}

	// Call LoadLibraryA remotely and pass our string (DLL path) as argument to it.
	if (!CreateRemoteThread(process_handle, NULL, 0,
		(LPTHREAD_START_ROUTINE)load_library, remote_argument, 0, NULL))
	{
		CloseHandle(process_handle);
		return error_code::CANNOT_CREATE_REMOTE_THREAD;
	}

	CloseHandle(process_handle);
	return error_code::NO_ERR;
}

} // namespace