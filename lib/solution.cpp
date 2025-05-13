#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <vector>

#include "solution.h"
#include "server.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

std::string solution::measure_data_file_path;
std::chrono::steady_clock::time_point solution::lastImgTime = std::chrono::steady_clock::now();
std::chrono::steady_clock::time_point solution::lastPositionTime = std::chrono::steady_clock::now();

const int chunkSize = 3000;

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
	if (!TcpServer::getInstance()->current_connection_) return;

	auto now = std::chrono::steady_clock::now();
	std::chrono::milliseconds interval(200);

	if (now - lastImgTime >= interval) {
		lastImgTime = now;
		//std::cout << "Processing image data..." << std::endl;

		BITMAPFILEHEADER* bmfh = (BITMAPFILEHEADER*)image;
		BITMAPINFOHEADER* bmih = (BITMAPINFOHEADER*)(image + sizeof(BITMAPFILEHEADER));
		uint8_t* bits = (uint8_t*)(image + bmfh->bfOffBits);
		int width = bmih->biWidth, height = bmih->biHeight, channels = bmih->biBitCount / 8;
		auto write_callback = [](void* context, void* data, int size) {
			auto* buf = reinterpret_cast<std::vector<uint8_t>*>(context);
			buf->insert(buf->end(), (uint8_t*)data, (uint8_t*)data + size);
		};

		std::vector<uint8_t> outMessage;
		stbi_write_jpg_to_func(write_callback, &outMessage, width, height, channels, bits, 90); // 90为压缩质量

		TcpServer* server = TcpServer::getInstance();
		int totalSize = outMessage.size();
		uint8_t batchSize = totalSize / chunkSize + (totalSize % chunkSize != 0 ? 1 : 0);
		uint8_t* data = new uint8_t[2];
		// 图片刷新信号
		std::cout << "new img batch size: " << static_cast<int>(batchSize) << std::endl;
		data[0] = 4; data[1] = batchSize;
		server->sendMessage(data, 2);
		delete[] data;
		
		data = new uint8_t[chunkSize+5];
		data[0] = 1;
		data[1] = batchSize;
		for (int i = 0; i < outMessage.size(); i+=chunkSize) {
			data[2] = static_cast<uint8_t>(i / chunkSize);
			size_t bytesToCopy = std::min(chunkSize, totalSize - i);
			data[3] = static_cast<uint8_t>(bytesToCopy & 0xFF);         
			data[4] = static_cast<uint8_t>((bytesToCopy >> 8) & 0xFF); 
			memcpy(data + 5, outMessage.data() + i, bytesToCopy);
			server->sendMessage(data, bytesToCopy + 5);
			std::cout << "send clip of " << bytesToCopy << " bytes; total size: " << totalSize << std::endl;
		}

		delete[] data;
	}
}

void solution::sendCurrentPosition(float x, float y, float z) {
	if (!TcpServer::getInstance()->current_connection_) return;

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