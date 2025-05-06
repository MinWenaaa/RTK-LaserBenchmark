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
	static std::chrono::steady_clock::time_point lastProcessedTime;

	void poccessImg(std::vector<unsigned char> image);
};
