#pragma once
#include <string>
#include <chrono>

class solution {
private:
	solution() {
	}
	solution(const solution&) = delete;
	solution& operator=(const solution) = delete;

public:
	static solution& getInstance() {
		static solution instance;
		return instance;
	}

	void initial();
	static std::string measure_data_file_path;
	static std::chrono::steady_clock::time_point lastImgTime;
	static std::chrono::steady_clock::time_point lastPositionTime;

	void poccessImg(const uint8_t* image);
	void sendCurrentPosition(float x, float y, float z);
	void sendMeasurement(double x, double y, double z, const char* name);
};
