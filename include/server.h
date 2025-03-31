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

    void start() {};
    void close() {};

private:
	TcpConnection(boost::asio::io_context& io_context)
		: socket_(io_context) {
	}
	void handleRead();
	void handleWrite(const std::string& message);
	boost::asio::ip::tcp::socket socket_;
	std::array<char, 1024> buffer_;

};

class TcpServer {
private:
    TcpServer();
    ~TcpServer() {}
    TcpServer(const TcpServer&) = delete; 
    TcpServer& operator=(const TcpServer&) = delete;

    static TcpServer* instance;

    void start_accept();

    boost::asio::io_context& io_context_;
    boost::asio::ip::tcp::acceptor acceptor_;
    boost::shared_ptr<TcpConnection> current_connection_;

public:
    static TcpServer* getInstance() {
        if (!instance) {
            instance = new TcpServer();
        }
        return instance;
    }

    void handle_accept(TcpConnection::pointer new_connection,
        const boost::system::error_code& error);
};