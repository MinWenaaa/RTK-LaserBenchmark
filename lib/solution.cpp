#include<iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <vector>

#include "solution.h"
#include "server.h"

std::string solution::measure_data_file_path;
std::chrono::steady_clock::time_point solution::lastProcessedTime = std::chrono::steady_clock::now();

void solution::initial() {
	auto now = std::chrono::system_clock::now();
	auto time = std::chrono::system_clock::to_time_t(now);
	std::tm tm;
	localtime_s(&tm, &time); // 使用 localtime_s 代替 localtime
	std::ostringstream oss;
	oss << std::put_time(&tm, "%Y%m%d%H%M");
	measure_data_file_path = "data" + oss.str() + ".txt";
	std::ofstream file(solution::measure_data_file_path, std::ios::out | std::ios::app);
	if (file.is_open()) {
		file << this->measure_data_file_path << std::endl;
		file.close();
	}
	else {
		std::cerr << "Unable to open file for writing. " << this->measure_data_file_path << std::endl;
	}
}

void solution::poccessImg(std::vector<unsigned char> image) {
	auto now = std::chrono::steady_clock::now();

	// 设定处理间隔（例如 1 秒）
	std::chrono::seconds interval(1);

	if (now - lastProcessedTime >= interval) {
		lastProcessedTime = now;

		std::cout << "Processing image data..." << std::endl;

		std::cout << "Image size: " << image.size() << " bytes" << std::endl;

		boost::asio::io_context io_context;
		TcpServer server(io_context);
		server.sendMessage("Processing image data...");
	}
}