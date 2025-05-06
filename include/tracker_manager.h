#pragma once
#include <string>
#include <chrono>
#include <vector>
#include "solution.h"
using namespace System;
using namespace LMF::Tracker;


void ConnectTo(const char* ipAdress);

void measure(const std::string& pointNum);

ref class TrackerManager {
public:
	static Tracker^ LMFTracker = nullptr;
	static MeasurementResults::Measurement^ LastMeasurement = nullptr;

	static void OnImageArrived(LMF::Tracker::OVC::OverviewCamera^ sender, array<System::Byte>^% image, OVC::ATRCoordinateCollection^ atrcoordinates) {
		std::vector<unsigned char> nativeArray;
		cli::pin_ptr<System::Byte> pinnedArray = &image[0];
		unsigned char* nativePtr = pinnedArray;
		nativeArray.assign(nativePtr, nativePtr + image->Length);

		solution::getInstance().poccessImg(nativeArray);
	}
};