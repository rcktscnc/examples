#include "user.h"
#include <exception>

#define NAME_BUF_SIZE 25

namespace chat
{

user::user(std::shared_ptr<tool::socket>& in,
	std::shared_ptr<tool::socket>& out,
	std::shared_ptr<tool::socket>& out_secondary) :
	_in(in), _out(out), _out_secondary(out_secondary)
{
	_marked_for_removal = false;
}

void user::mark_for_removal()
{
	_marked_for_removal = true;
}

bool user::is_marked_for_removal() const
{
	return _marked_for_removal;
}

tool::size_t user::id() const
{
	return _id;
}

std::string user::name() const
{
	return _user_name;
}

int user::in(tool::buffer* buf, tool::error_code* err)
{
	return _in->read(buf, err);
}

int user::out(tool::buffer* buf, tool::error_code* err)
{
	return _out->write(buf, err);
}

int user::out_secondary(tool::buffer* buf, tool::error_code* err)
{
	return _out_secondary->write(buf, err);
}

void user::close()
{
	_in->close();
	_out->close();
	_out_secondary->close();
}

bool user::init_user_name()
{
	tool::buffer buf(NAME_BUF_SIZE);

	try
	{
		// You should test err here
		tool::error_code err;
		_in->read(&buf, &err);
		if (err != tool::error_code::NO_ERR)
		{
			printf("error reading user name: %s\n", tool::error_to_string(err));
			return false;
		}
		
		_user_name = (const char*)buf.data();
	}
	catch (const std::exception& e)
	{
		printf("exception caught reading user name: %s\n", e.what());
		return false;
	}
	
	return true;
}

bool user::init_connection()
{
	// TODO: receive one message from each node to confirm its purpose?
	const size_t buf_size = 46;
	char buf[buf_size] = { 0 };

	_in->remote_address(buf, buf_size);
	std::string node1(buf);

	_out->remote_address(buf, buf_size);
	std::string node2(buf);

	_out_secondary->remote_address(buf, buf_size);
	std::string node3(buf);

	return (node1 == node2 && node2 == node3) && init_user_name();
}

} // namespace