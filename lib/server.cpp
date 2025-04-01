#include <iostream>
#include <boost/bind/bind.hpp>

#include "server.h"

void TcpConnection::start() {
	handleRead();
}

void TcpConnection::handleRead() {
	auto self(shared_from_this());
	socket_.async_read_some(boost::asio::buffer(buffer_),
		[this, self](boost::system::error_code ec, std::size_t length) {
			if (!ec) {
				std::string message(buffer_.data(), length);
				std::cout << "Received: " << message << std::endl;
				handleRead();
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
		boost::asio::ip::tcp::endpoint remote_endpoint = new_connection->socket().remote_endpoint();
		std::cout << "New connection from: " << remote_endpoint.address().to_string()
			<< ":" << remote_endpoint.port() << std::endl;
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

void TcpServer::start(short port) {
	boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), port);
	acceptor_.open(endpoint.protocol());
	acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
	acceptor_.bind(endpoint);
	acceptor_.listen();
	start_accept();
}

void TcpServer::stop() {
	acceptor_.close();
}

void TcpServer::sendMessage(const std::string& message) {
	if (current_connection_) {
		current_connection_->handleWrite(message);
	}
	else {
		std::cerr << "No active connection to send message" << std::endl;
	}
}