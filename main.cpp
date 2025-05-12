#include <iostream>
#include <vcclr.h>

#include "tracker_manager.h"
#include "server.h"

using namespace System;

const char* ipAddress = "ATS500Simulator";
//const char* ipAddress = "192.168.250.1";

int main() {
	solution::getInstance().initial();
	ConnectTo("AT500Simulator");
	boost::asio::io_context io_context;
	TcpServer server(io_context);
	server.start(3001);

	std::thread io_thread([&io_context]() {
		io_context.run();
		});

	while (true) {
		std::string message;
		std::getline(std::cin, message);
		if (message == "exit") {
			break;
		}
		std::vector<unsigned char> vec(message.begin(), message.end());
		server.sendMessage(vec);
	}

	io_context.stop();
	io_thread.join();

	return 0;
}
#pragma managed(pop)