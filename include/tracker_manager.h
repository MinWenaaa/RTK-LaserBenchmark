#pragma once
using namespace System;

ref class TrackerManager {
private:
	static TrackerManager^ instance = nullptr;
	TrackerManager() {}

public:
	static property TrackerManager^ getInstance {
		TrackerManager^ get() {
			if (instance == nullptr) {
				instance = gcnew TrackerManager();
			}
			return instance;
		}
	}


	static LMF::Tracker::Tracker^ LMFTracker = nullptr;

	void ConnectTo(const char* ipAdress);
};