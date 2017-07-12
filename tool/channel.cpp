#include "channel.h"
#include <algorithm>
#include <cstdio>
#include <chrono>

#define BUF_SIZE 512

namespace chat
{

using tool::size_t;

channel::channel() :
	messages_sender_thread(std::make_unique<std::thread>(&channel::messages_sender, this)),
	current_users_sender_thread(std::make_unique<std::thread>(&channel::current_users_sender, this))
{
}

void channel::add_user(std::shared_ptr<chat::user>& user)
{
	if (!user->init_connection())
		return;

	std::lock_guard<std::mutex> guard(users_mutex);
	users.push_back(user);
	receivers.push_back(std::thread(&channel::messages_receiver, this, user));
}

void channel::remove_users()
{
	auto predicate = [](const std::shared_ptr<chat::user>& user) -> bool
	{
		if (user->is_marked_for_removal())
		{
			user->close(); // make sure messages_receiver() function will return.
			return true;
		}

		return false;
	};

	users.erase(std::remove_if(users.begin(), users.end(), predicate), users.end());
}

void channel::current_users_sender()
{
	tool::buffer all_names_buf(BUF_SIZE);
	while (true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		std::lock_guard<std::mutex> guard(users_mutex);

		if (users.empty())
			continue;

		std::string all_names;
		for (auto& u : users)
			all_names += u->name() + "\n";

		all_names_buf.pack(all_names.c_str());
		bool pending_removal = false;

		for (auto& u : users)
		{
			tool::error_code err;
			u->out_secondary(&all_names_buf, &err);
			if (err != tool::error_code::NO_ERR)
			{
				printf("current_users_sender error_code: %s\n", tool::error_to_string(err));
				u->mark_for_removal();
				pending_removal = true;
			}
		}

		if (pending_removal)
			remove_users();
	}
}

void channel::messages_sender()
{
	while (true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		std::lock_guard<std::mutex> guard1(users_mutex);
		std::lock_guard<std::mutex> guard2(messages_mutex);

		if (users.empty() || messages.empty())
			continue;
		
		bool pending_removal = false;

		for (auto& u : users)
		{
			for (auto& m : messages)
			{
				tool::error_code err;
				u->out(m.get(), &err);
				if (err != tool::error_code::NO_ERR)
				{
					printf("messages_sender error_code: %s\n", tool::error_to_string(err));
					u->mark_for_removal();
					pending_removal = true;
					break;
				}
			}
		}

		if (pending_removal)
			remove_users();
		
		messages.clear();
	}
}

std::string channel::init_user_name(tool::buffer& buf,
	std::shared_ptr<chat::user>& user)
{
	std::lock_guard<std::mutex> guard1(users_mutex);
	std::lock_guard<std::mutex> guard2(messages_mutex);

	std::string greeting("[ " + user->name() + " joined ]");
	buf.pack(greeting.c_str());

	auto dynamic_buf = std::make_shared<tool::buffer>(greeting.size() + 1); // size + null character.
	dynamic_buf->pack(greeting.c_str());
	messages.push_back(dynamic_buf);

	printf("user connected: %s\n", user->name().c_str());
	return user->name();
}

void channel::messages_receiver(std::shared_ptr<chat::user> user)
{
	tool::buffer buf(BUF_SIZE);
	std::string user_name = init_user_name(buf, user);
	while (true)
	{
		tool::error_code err;
		user->in(&buf, &err);

		std::lock_guard<std::mutex> guard(messages_mutex);

		if (err != tool::error_code::NO_ERR)
		{
			printf("user disconnected: %s\n", user_name.c_str());
			printf("messages_receiver error_code: %s\n", tool::error_to_string(err));

			std::string goodbye("[ " + user_name + " left ]");
			auto dynamic_buf = std::make_shared<tool::buffer>(goodbye.size() + 1);  // size + null character.
			dynamic_buf->pack(goodbye.c_str());
			messages.push_back(dynamic_buf);
			return;
		}

		auto dynamic_buf = std::make_shared<tool::buffer>(buf.size());
		dynamic_buf->pack(&buf);
		messages.push_back(dynamic_buf);
	}
}

} // namespace