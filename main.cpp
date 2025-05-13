#include <iostream>
#include <vcclr.h>
#include "tracker_manager.h"
#include "server.h"
#include "solution.h"

using namespace System;


int main() {
	solution::getInstance().initial();
	ConnectTo("AT500Simulator");

	auto io_context = std::make_shared<boost::asio::io_context>();
	TcpServer::getInstance()->initial(*io_context);
	TcpServer* server = TcpServer::getInstance();
	server->start(3001);

	std::thread io_thread([&io_context]() {
		try {
			io_context->run();
		}
		catch (const std::exception& e) {
			std::cerr << "IO thread exception: " << e.what() << std::endl;
		}
	});

	while (true) {
		std::string message;
		std::getline(std::cin, message);
		if (message == "exit") {
			break;
		}
		//server->sendMessage(message);
	}

	io_context->stop();
	io_thread.join();

	return 0;
}
#pragma managed(pop)