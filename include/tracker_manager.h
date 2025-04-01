#pragma once
using namespace System;

ref class TrackerManager {
public:

	static LMF::Tracker::Tracker^ LMFTracker = nullptr;

};

void ConnectTo(const char* ipAdress);