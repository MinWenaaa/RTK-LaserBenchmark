#include <iostream>
#include <boost/bind/bind.hpp>

#include "server.h"

void TcpConnection::handleRead() {
	auto self(shared_from_this());
	socket_.async_read_some(boost::asio::buffer(buffer_),
		[this, self](boost::system::error_code ec, std::size_t length) {
			if (!ec) {
				std::string message(buffer_.data(), length);
				std::cout << "Received: " << message << std::endl;
			} else {
				if (ec == boost::asio::error::eof) {
					std::cout << "Connection closed by peer" << std::endl;
				} else {
					std::cerr << "Error: " << ec.message() << std::endl;
				}
				close();
			}
		}
	);
}

void TcpConnection::handleWrite(const std::string& message) {
	auto self(shared_from_this());
	boost::asio::async_write(socket_, boost::asio::buffer(message),
		[this, self](boost::system::error_code ec, std::size_t length) {
			if (ec) {
				std::cerr << "Error: " << ec.message() << std::endl;
				close();
			}
		}
	);
}

void TcpServer::start_accept() {
	TcpConnection::pointer new_connection = TcpConnection::create(io_context_);
	acceptor_.async_accept(new_connection->socket(),
		boost::bind(&TcpServer::handle_accept, this, new_connection,
		boost::asio::placeholders::error));
}

void TcpServer::handle_accept(TcpConnection::pointer new_connection,
	const boost::system::error_code& error) {
	if (!error) {
		if (current_connection_) {
			std::cout << "Connection already exists" << std::endl;
			new_connection->close();
		} else {
			current_connection_ = new_connection;
			current_connection_->start();
		}
	} else {
		std::cerr << "Error: " << error.message() << std::endl;
	}
	start_accept();
}