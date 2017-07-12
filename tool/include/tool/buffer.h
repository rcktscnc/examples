#ifndef _TOOL_BUFFER_H
#define _TOOL_BUFFER_H

#include <tool/types.h>
#include <tool/error_code.h>

namespace tool
{

class buffer
{
	friend class socket;
public:
	buffer& operator= (const buffer&) = delete;
	
	buffer(size_t capacity) noexcept(false);
	buffer(const buffer& rhs) noexcept(false);
	buffer(buffer&& rhs) noexcept;
	buffer& operator= (buffer&& rhs) noexcept;
	~buffer() noexcept;

	error_code pack(byte_t* buf, size_t size) noexcept;
	error_code pack(const char* buf) noexcept;
	error_code pack(buffer* buf) noexcept;

	byte_t* data() noexcept;
	size_t size() const noexcept;
	size_t capacity() noexcept;
	void set_size(size_t size) noexcept;

private:
	byte_t* _buf;
	size_t _capacity;
};

} // namespace

#endif

