#ifndef _CHAT_USER_H
#define _CHAT_USER_H

#include <tool/net.h>
#include <mutex>
#include <memory>
#include <string>

namespace chat
{

class user
{
public:
	user(std::shared_ptr<tool::socket>& in,
		std::shared_ptr<tool::socket>& out,
		std::shared_ptr<tool::socket>& out_secondary);

	tool::size_t id() const;
	std::string name() const;
	
	bool is_marked_for_removal() const;
	void mark_for_removal();

	bool init_connection();
	
	int in(tool::buffer* buf, tool::error_code* err);
	int out(tool::buffer* buf, tool::error_code* err);
	int out_secondary(tool::buffer* buf, tool::error_code* err);

	void close();

private:
	bool _marked_for_removal;
	tool::size_t _id;
	std::string _user_name;
	
	std::shared_ptr<tool::socket> _in;
	std::shared_ptr<tool::socket> _out;
	std::shared_ptr<tool::socket> _out_secondary;
	
	bool init_user_name();
};

} // namespace

#endif