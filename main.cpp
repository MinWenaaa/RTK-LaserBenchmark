#include <iostream>
#include <vcclr.h>

#include "tracker_manager.h"
#include "server.h"

using namespace System;

int main() {
	ConnectTo("ATS600Simulator");
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
		server.sendMessage(message);
	}

	io_context.stop();
	io_thread.join();

	return 0;
}
#pragma managed(pop)