#include <tool/net.h>
#include <thread>
#include <cstdio>

#include <string>
#include <iostream>

#define BUF_SIZE 512
#define ADDRESS "localhost"
#define PORT "22335"

void receiver(tool::socket* sckt)
{
	while (1)
	{
		tool::buffer buf(BUF_SIZE);
		tool::error_code err;
		sckt->read(&buf, &err);
		if (err != tool::error_code::NO_ERR)
		{
			printf("read error_code: %s\n", tool::error_to_string(err));
			return;
		}

		printf("%s%s", (char*)buf.data(), "\n");
	}
}

int _main(int argc, const char* argv[])
{
	if (argc != 2)
	{
		printf("usage: binary_name name\n");
		return -1;
	}
	
	tool::net_start();

	tool::socket from_server;
	tool::socket to_server;
	
	tool::error_code err = tool::connect(&to_server, ADDRESS, PORT);
	if (err != tool::error_code::NO_ERR)
	{
		printf("Couldn't connect to node 1: %s\n", tool::error_to_string(err));
		return -1;
	}
	
	err = tool::connect(&from_server, ADDRESS, PORT);
	if (err != tool::error_code::NO_ERR)
	{
		printf("Couldn't connect to node 2: %s\n", tool::error_to_string(err));
		return -1;
	}
	
	tool::buffer msg(BUF_SIZE);
	msg.pack(argv[1]);
	to_server.write(&msg);

	std::thread receive_messages(receiver, &from_server);

	while (1)
	{
		std::string msg(argv[1]);
		msg += ": ";

		std::string input;
		std::getline(std::cin, input, '\n');

		msg += input;

		tool::buffer buf(BUF_SIZE);

		buf.pack((tool::byte_t*)msg.c_str(), msg.length() + 1);

		tool::error_code err;
		to_server.write(&buf, &err);
		if (err != tool::error_code::NO_ERR)
		{
			printf("write error_code: %s\n", tool::error_to_string(err));
			return -1;
		}
	}

	tool::net_stop();
	return 0;
}