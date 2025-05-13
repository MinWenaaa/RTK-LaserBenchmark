#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>

#include "solution.h"
#include "server.h"

std::string solution::measure_data_file_path;
std::chrono::steady_clock::time_point solution::lastImgTime = std::chrono::steady_clock::now();
std::chrono::steady_clock::time_point solution::lastPositionTime = std::chrono::steady_clock::now();

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
	} else {
		std::cerr << "Unable to open file for writing. " << this->measure_data_file_path << std::endl;
	}
}

void solution::poccessImg(const uint8_t* image) {
	auto now = std::chrono::steady_clock::now();
	std::chrono::seconds interval(5);

	if (now - lastImgTime >= interval) {
		lastImgTime = now;
		//std::cout << "Processing image data..." << std::endl;

		BITMAPFILEHEADER* bmfh = (BITMAPFILEHEADER*)image;
		BITMAPINFOHEADER* bmih = (BITMAPINFOHEADER*)(image + sizeof(BITMAPFILEHEADER));
		uint8_t* bits = (uint8_t*)(image + bmfh->bfOffBits);

		// 处理图像数据

		//uint32_t bitmapDataSize = bmfh->bfSize;
		//uint32_t packetSize = sizeof(uint32_t) * 2 + bitmapDataSize;
		//char* packet = new char[packetSize];

		//// 填充数据包
		//uint8_t type = 1; // 1 表示位图数据
		//memcpy(packet, &type, sizeof(uint32_t));
		//memcpy(packet + sizeof(uint32_t), &bitmapDataSize, sizeof(uint32_t));
		//memcpy(packet + sizeof(uint32_t) + sizeof(uint32_t), image, bitmapDataSize);

		//// 发送数据包
		//TcpServer* server = TcpServer::getInstance();
		//server->sendMessage(packet, packetSize);
		//std::cout << packetSize << std::endl;
		//delete[] packet;
	}
}

void solution::sendCurrentPosition(float x, float y, float z) {
	auto now = std::chrono::steady_clock::now();
	std::chrono::milliseconds interval(100);
	if (now - lastPositionTime >= interval) {
		lastPositionTime = now;

		//std::cout << "Processing position data..." << std::endl;

		uint8_t type = 2;
		uint8_t* data = new uint8_t[sizeof(float) * 3 + sizeof(uint8_t)];
		memcpy(data, &type, sizeof(uint8_t));
		memcpy(data + sizeof(uint8_t), &x, sizeof(float));
		memcpy(data + sizeof(uint8_t) + sizeof(float), &y, sizeof(float));
		memcpy(data + sizeof(uint8_t) + sizeof(float) * 2, &z, sizeof(float));

		TcpServer* server = TcpServer::getInstance();
		server->sendMessage(data, sizeof(float) * 3 + sizeof(uint8_t));
		delete[] data;
	}
}

void solution::sendMeasurement(double x, double y, double z, const char* name) {
	std::ofstream file(solution::measure_data_file_path, std::ios::out | std::ios::app);
	if (file.is_open()) {
		std::cout << "Processing position data..." << std::endl;
		file << "add " << name << " " << x << "," << y << " " << z << std::endl;
		file.close();
	} else {
		std::cerr << "Unable to open file for writing. " << this->measure_data_file_path << std::endl;
	}
	uint8_t type = 3;
	uint8_t nameLen = strlen(name);
	int dataLen = sizeof(uint8_t) * 2 + sizeof(char) * nameLen + sizeof(double) * 3;
	uint8_t* data = new uint8_t[dataLen];

	memcpy(data, &type, sizeof(uint8_t));
	memcpy(data + sizeof(uint8_t), &nameLen, sizeof(double));
	memcpy(data + sizeof(uint8_t) * 2, name, sizeof(char) * nameLen);
	memcpy(data + sizeof(uint8_t) * 2 + sizeof(char) * nameLen, &x, sizeof(double));
	memcpy(data + sizeof(uint8_t) * 2 + sizeof(char) * nameLen + sizeof(double), &y, sizeof(double));
	memcpy(data + sizeof(uint8_t) * 2 + sizeof(char) * nameLen + sizeof(double) * 2, &z, sizeof(double));

	TcpServer* server = TcpServer::getInstance();
	server->sendMessage(data, dataLen);
	delete[] data;
}