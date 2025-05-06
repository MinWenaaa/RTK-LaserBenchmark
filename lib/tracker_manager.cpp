#include <cstring>
#include <fstream>
#include <iostream>’
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

std::string solution::measure_data_file_path;
std::chrono::steady_clock::time_point solution::lastProcessedTime = std::chrono::steady_clock::now();

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
		std::cerr << "Unable to open file for writing. " << this->measure_data_file_path  << std::endl;
	}
}

void solution::poccessImg(array<System::Byte>^ image) {
	auto now = std::chrono::steady_clock::now();

	// 设定处理间隔（例如 1 秒）
	std::chrono::seconds interval(1);

	if (now - lastProcessedTime >= interval) {
		lastProcessedTime = now;

		std::cout << "Processing image data..." << std::endl;

		std::cout << "Image size: " << image->Length << " bytes" << std::endl;

		cli::pin_ptr<System::Byte> pinnedArray = &image[0];
		unsigned char* nativeArray = pinnedArray;
		std::vector<unsigned char> data(nativeArray, nativeArray + image->Length);

		//TcpServer::getInstance()->sendMessage(data);
	}
}