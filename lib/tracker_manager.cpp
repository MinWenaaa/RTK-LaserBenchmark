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

void ConnectTo(const char* ipAddress) {
	if (TrackerManager::LMFTracker) TrackerManager::LMFTracker->Disconnect();

	Connection^ con = gcnew Connection();
	if (strcmp(ipAddress, "0.0.0.0")) {
		TrackerManager::LMFTracker = con->Connect(gcnew System::String(ipAddress));

		// 注册事件
		TrackerManager::LMFTracker->OverviewCamera->ImageArrived += gcnew LMF::Tracker::OVC::OverviewCamera::ImageArrivedHandler(&TrackerManager::OnImageArrived);
		TrackerManager::LMFTracker->OverviewCamera->StartAsync();
		TrackerManager::LMFTracker->Targets->TargetPositionChanged += gcnew LMF::Tracker::Targets::TargetCollection::TargetPositionChangedHandler(&TrackerManager::OnTargetPostionChanged);
		TrackerManager::LMFTracker->Measurement->MeasurementArrived += gcnew LMF::Tracker::Measurements::MeasurementSettings::MeasurementArrivedHandler(&TrackerManager::OnMeasurementArrived);
	}
}

#include <msclr/marshal.h> // Add this include for marshaling  

void handleCommand(const uint8_t* data, std::size_t size) {  
    std::cout << "receive measure command " << data[0] << "!" << std::endl;
    std::ofstream file(solution::measure_data_file_path, std::ios::out | std::ios::app);
    switch (data[0]) {  
    case 1:  
        // 开始测量  
        if (TrackerManager::LMFTracker == nullptr) return;  
        try {  
            if (TrackerManager::LMFTracker->Measurement->Status->Value == EMeasurementStatus::ReadyToMeasure) {  
                LMF::Tracker::MeasurementResults::Measurement^ LastMeasurement = TrackerManager::LMFTracker->Measurement->MeasureStationary();  
                if (StationaryMeasurement3D^ stationaryMeas3D = dynamic_cast<StationaryMeasurement3D^>(LastMeasurement)) {  
                    double coord1 = stationaryMeas3D->Position->Coordinate1->Value;  
                    double coord2 = stationaryMeas3D->Position->Coordinate2->Value;  
                    double coord3 = stationaryMeas3D->Position->Coordinate3->Value;  
                    solution::getInstance().sendMeasurement(coord1, coord2, coord3, reinterpret_cast<const char*>(data + 1));  
                }  
            }  
        }  
        catch (LmfException^ ex) {  
            // Use msclr::interop::marshal_as to convert System::String^ to std::string  
            std::string errorMessage = msclr::interop::marshal_as<std::string>(ex->Message);  
            std::cerr << "Measurement error: " << errorMessage << std::endl;  
        }  
        break;  
    case 2:  
        // 删除点
        if (file.is_open()) {
            std::cout << "deleting position data..." << std::endl;
            file << "del " << reinterpret_cast<const char*>(data + 1) << std::endl;
            file.close();
        } else {
            std::cerr << "Unable to open file for writing. " << solution::measure_data_file_path << std::endl;
        }
        break;  
    default:  
        std::cerr << "invalid command!" << std::endl;  
    }  

    if (file.is_open()) {
        file.close();
    }
}
