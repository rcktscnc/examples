#include <tool/buffer.h>
#include <cstring>

namespace tool
{

buffer::buffer(size_t capacity) noexcept(false)
	: _buf(new byte_t[capacity + sizeof(size_t)])
	, _capacity(capacity + sizeof(size_t))
{
}

buffer::buffer(const buffer& rhs) noexcept(false)
	: buffer(rhs._capacity - sizeof(size_t))
{
	set_size(rhs.size());
	std::memcpy(data(), rhs._buf + sizeof(size_t), rhs.size());
}

buffer::buffer(buffer&& rhs) noexcept : _capacity(rhs._capacity)
{
	_buf = rhs._buf;
	rhs._buf = nullptr;
}

buffer& buffer::operator= (buffer&& rhs) noexcept
{
	_capacity = rhs._capacity;
	_buf = rhs._buf;
	rhs._buf = nullptr;

	return *this;
}

buffer::~buffer() noexcept
{
	delete[] _buf;
}

error_code buffer::pack(byte_t* buf, size_t size) noexcept
{
	if (size + sizeof(size_t) > _capacity)
		return error_code::BUFFER_OUT_OF_RANGE;

	*(size_t*)_buf = size;
	std::memcpy(_buf + sizeof(size_t), buf, size);

	return error_code::NO_ERR;
}

error_code buffer::pack(const char* buf) noexcept
{
	return pack((byte_t*)buf, std::strlen(buf) + 1);
}

error_code buffer::pack(buffer* buf) noexcept
{
	return pack(buf->data(), buf->size());
}

byte_t* buffer::data() noexcept
{
	return _buf + sizeof(size_t);
}

size_t buffer::size() const noexcept
{
	return *(size_t*)_buf;
}

size_t buffer::capacity() noexcept
{
	return _capacity;
}

void buffer::set_size(size_t size) noexcept
{
	*(size_t*)_buf = size;
}

} // namespace

