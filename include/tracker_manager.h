#pragma once
#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>
using namespace System;
using namespace LMF::Tracker;


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
	static std::chrono::steady_clock::time_point lastProcessedTime;

	void poccessImg(array<System::Byte>^ image);
};

ref class TrackerManager {
public:
	static Tracker^ LMFTracker = nullptr;
	static MeasurementResults::Measurement^ LastMeasurement = nullptr;

	static void OnImageArrived(LMF::Tracker::OVC::OverviewCamera^ sender, array<System::Byte>^% image, OVC::ATRCoordinateCollection^ atrcoordinates) {
		solution::getInstance().poccessImg(image);
	}
};