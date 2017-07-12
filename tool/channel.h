#ifndef _CHAT_ROOM_H
#define _CHAT_ROOM_H

#include <tool/net.h>
#include "user.h"
#include <thread>
#include <mutex>
#include <vector>
#include <string>
#include <memory>

namespace chat
{

class channel
{
public:
	channel(const channel&) = delete;
	channel(channel&&) = delete;
	channel& operator= (const channel&) = delete;
	channel& operator= (channel&&) = delete;

	channel();

	void add_user(std::shared_ptr<chat::user>& user);

private:
	std::mutex users_mutex;
	std::vector<std::shared_ptr<chat::user>> users;

	std::mutex messages_mutex;
	std::vector<std::shared_ptr<tool::buffer>> messages;

	std::unique_ptr<std::thread> messages_sender_thread;
	std::unique_ptr<std::thread> current_users_sender_thread;
	
	std::vector<std::thread> receivers;

	void remove_users();
	void current_users_sender();
	void messages_sender();
	std::string init_user_name(tool::buffer& buf, std::shared_ptr<chat::user>& user);
	void messages_receiver(std::shared_ptr<chat::user> from_user);
};

} // namespace

#endif