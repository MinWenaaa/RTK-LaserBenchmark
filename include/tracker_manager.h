#pragma once  
#include <string>  
#include <chrono>  
#include <vector>  
#include "solution.h"  

using namespace System;  
using namespace LMF::Tracker;  
using namespace LMF::Tracker::Measurements;
using namespace LMF::Tracker::MeasurementResults;
using namespace LMF::Tracker::Enums;
using namespace LMF::Tracker::ErrorHandling;
using namespace LMF::Tracker::Targets;

void ConnectTo(const char* ipAdress);  

void handleCommand(const uint8_t*, std::size_t);  

ref class TrackerManager {  
public:  
    static Tracker^ LMFTracker = nullptr;  
    static MeasurementResults::Measurement^ LastMeasurement = nullptr;  

    // 影像数据流  
    static void OnImageArrived(LMF::Tracker::OVC::OverviewCamera^ sender, array<System::Byte>^% image, OVC::ATRCoordinateCollection^ atrcoordinates) {  
	    pin_ptr<Byte> pinnedImage = &image[0];  
	    uint8_t* nativeImage = reinterpret_cast<uint8_t*>(pinnedImage);  
	    solution::getInstance().poccessImg(nativeImage);  
    }  

    // 坐标数据流  
    static void OnTargetPostionChanged(LMF::Tracker::Tracker^ sender, LMF::Tracker::MeasurementResults::SingleShotMeasurement3D^ position) {
	    float coord1 = position->Position->Coordinate1->Value;  
	    float coord2 = position->Position->Coordinate2->Value;
	    float coord3 = position->Position->Coordinate3->Value;

	    solution::getInstance().sendCurrentPosition(coord1, coord2, coord3);  
    }  

   static void OnMeasurementArrived(MeasurementSettings^ sender, MeasurementCollection^ measurements, LmfException^ exception) {
       std::cout << "onMeasurementArrived!" << std::endl;
      if (measurements != nullptr && measurements->Count > 0) {   
          StationaryMeasurement3D^ stationaryMeas3D = dynamic_cast<StationaryMeasurement3D^>(measurements[0]);
          if (stationaryMeas3D != nullptr) {  
              // 处理测量数据  
              double coord1 = stationaryMeas3D->Position->Coordinate1->Value;  
              double coord2 = stationaryMeas3D->Position->Coordinate2->Value;
              double coord3 = stationaryMeas3D->Position->Coordinate3->Value;
              solution::getInstance().sendCurrentPosition(coord1, coord2, coord3);  
          }  
      }  
      else if (exception != nullptr) {  

      }  
   }
};