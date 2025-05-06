#include <cstring>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <direct.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <msclr/marshal_cppstd.h>
#include "tracker_manager.h"

using namespace LMF::Tracker;
using namespace LMF::Tracker::Enums;
using namespace LMF::Tracker::ErrorHandling;
using namespace LMF::Tracker::Measurements;
using namespace LMF::Tracker::MeasurementResults;

void ConnectTo(const char* ipAddress) {
	if (TrackerManager::LMFTracker) TrackerManager::LMFTracker->Disconnect();

	Connection^ con = gcnew Connection();
	if (strcmp(ipAddress, "0.0.0.0")) {
		TrackerManager::LMFTracker = con->Connect(gcnew System::String(ipAddress));
		TrackerManager::LMFTracker->OverviewCamera->ImageArrived += gcnew LMF::Tracker::OVC::OverviewCamera::ImageArrivedHandler(&TrackerManager::OnImageArrived);
		TrackerManager::LMFTracker->OverviewCamera->StartAsync();
	}
}

void measure(const std::string& pointName) {
	std::cout << "point " << pointName << " start measurement" << " at " << solution::measure_data_file_path << std::endl;
	try {
		if (TrackerManager::LMFTracker->Measurement->Status->Value == EMeasurementStatus::ReadyToMeasure) {
			TrackerManager::LastMeasurement = TrackerManager::LMFTracker->Measurement->MeasureStationary();
			StationaryMeasurement3D^ stationaryMeas3D = dynamic_cast<StationaryMeasurement3D^>(TrackerManager::LastMeasurement);

			String^ x = stationaryMeas3D->Position->Coordinate1->ToString();
			String^ y = stationaryMeas3D->Position->Coordinate2->ToString();
			String^ z = stationaryMeas3D->Position->Coordinate3->ToString();

			std::string x_str = msclr::interop::marshal_as<std::string>(x);
			std::string y_str = msclr::interop::marshal_as<std::string>(y);
			std::string z_str = msclr::interop::marshal_as<std::string>(z);

			std::ofstream file(solution::measure_data_file_path, std::ios_base::app);
			if (file.is_open()) {
				file << "Point: " << pointName << ", X: " << x_str << ", Y: " << y_str << ", Z: " << z_str << std::endl;
				file.close();
				std::cout << "Measurement " << pointName << " written to file." << std::endl;
			}
			else {
				std::cerr << "Unable to open file for writing." << std::endl;
			}
		} 
	}
	catch (LmfException^ ex) {
		
	}
}
