#pragma once
#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>

class TcpConnection: public boost::enable_shared_from_this<TcpConnection> {
public:
    typedef boost::shared_ptr<TcpConnection> pointer;
	static pointer create(boost::asio::io_context& io_context) {
		return pointer(new TcpConnection(io_context));
	}
    boost::asio::ip::tcp::socket& socket() {
		return socket_;
	}

    void start();
    void close() {};

    void handleWrite(const std::vector<unsigned char>& message);

private:
	TcpConnection(boost::asio::io_context& io_context)
		: socket_(io_context) {
	}
	void handleRead(std::function<void(const std::string&)> callback);

	boost::asio::ip::tcp::socket socket_;
	std::array<char, 1024> buffer_;

};

class TcpServer {
private:
    void start_accept();
    void handle_accept(TcpConnection::pointer new_connection,
        const boost::system::error_code& error);

    boost::asio::io_context& io_context_;
    boost::asio::ip::tcp::acceptor acceptor_;
    boost::shared_ptr<TcpConnection> current_connection_;

public:
    TcpServer(boost::asio::io_context& io_context)
        : io_context_(io_context), acceptor_(io_context) {
    }
    ~TcpServer() { stop(); }


    void start(short port);
    void stop();

    void sendMessage(const std::vector<unsigned char>& message);
};