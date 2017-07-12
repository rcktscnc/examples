#include "channel.h"

std::shared_ptr<chat::user> accept_user(tool::acceptor& acceptor)
{
	const size_t buf_size = 46;
	char buf[buf_size] = { 0 };

	auto from_user = std::make_shared<tool::socket>();
	auto to_user = std::make_shared<tool::socket>();
	auto to_user_names = std::make_shared<tool::socket>();

	tool::error_code err = acceptor.accept(from_user.get());
	if (err != tool::error_code::NO_ERR)
		printf("node 1 error_code: %s\n", tool::error_to_string(err));

	from_user->remote_address(buf, buf_size);
	printf("node 1: %s:%d\n", buf, from_user->remote_port());

	err = acceptor.accept(to_user.get());
	if (err != tool::error_code::NO_ERR)
		printf("node 2 error_code: %s\n", tool::error_to_string(err));

	to_user->remote_address(buf, buf_size);
	printf("node 2: %s:%d\n", buf, to_user->remote_port());

	err = acceptor.accept(to_user_names.get());
	if (err != tool::error_code::NO_ERR)
		printf("node 3 error_code: %s\n", tool::error_to_string(err));

	to_user_names->remote_address(buf, buf_size);
	printf("node 3: %s:%d\n", buf, to_user_names->remote_port());

	return std::make_shared<chat::user>(from_user, to_user, to_user_names);
}

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		printf("usage: binary_name port\n");
		return -1;
	}

	tool::net_start();

	std::vector<chat::channel*> rooms;
	rooms.push_back(new chat::channel());

	tool::acceptor acceptor(argv[1]);

	printf("listening on port %s\n", argv[1]);

	while (true)
	{
		std::shared_ptr<chat::user> user = accept_user(acceptor);
		rooms.at(0)->add_user(user);
	}

	tool::net_stop();
	return 0;
}