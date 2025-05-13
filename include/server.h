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

    void handleWrite(const uint8_t* data, std::size_t size);

private:
	TcpConnection(boost::asio::io_context& io_context)
		: socket_(io_context) {
	}
	void handleRead(std::function<void(const uint8_t*, std::size_t)> callback);

	boost::asio::ip::tcp::socket socket_;
	std::array<char, 1024> buffer_;

};

class TcpServer {
private:
    // 私有构造函数，禁止外部实例化
    TcpServer() : io_context_(nullptr), acceptor_(nullptr) {}

    // 禁用拷贝构造和赋值操作符
    TcpServer(const TcpServer&) = delete;
    TcpServer& operator=(const TcpServer&) = delete;

    // 私有成员变量
    boost::asio::io_context* io_context_;
    boost::asio::ip::tcp::acceptor* acceptor_;

    // 静态实例
    static TcpServer* instance_;
    static std::mutex instance_mutex_;

    void start_accept();
    void handle_accept(TcpConnection::pointer new_connection,
        const boost::system::error_code& error);

public:
    // 获取单例实例
    static TcpServer* getInstance() {
        std::lock_guard<std::mutex> lock(instance_mutex_);
        if (instance_ == nullptr) {
            instance_ = new TcpServer();
        }
        return instance_;
    }

    boost::shared_ptr<TcpConnection> current_connection_;

    // 初始化方法
    void initial(boost::asio::io_context& io_context) {
        io_context_ = &io_context;
        acceptor_ = new boost::asio::ip::tcp::acceptor(*io_context_);
    }

    // 公共方法
    void start(short port);
    void stop();
    void sendMessage(const uint8_t* data, std::size_t size);
};