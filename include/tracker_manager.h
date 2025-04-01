#pragma once
#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>
using namespace System;

ref class TrackerManager {
public:
	static LMF::Tracker::Tracker^ LMFTracker = nullptr;
	static LMF::Tracker::MeasurementResults::Measurement^ LastMeasurement = nullptr;
};

void ConnectTo(const char* ipAdress);

void measure(const std::string& pointNum);

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
};