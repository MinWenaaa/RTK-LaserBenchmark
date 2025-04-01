#include <cstring>
#include "tracker_manager.h"

using namespace LMF::Tracker;

void ConnectTo(const char* ipAddress) {
	if (TrackerManager::LMFTracker) TrackerManager::LMFTracker->Disconnect();

	Connection^ con = gcnew Connection();
	if (strcmp(ipAddress, "0.0.0.0")) {
		TrackerManager::LMFTracker = con->Connect(gcnew System::String(ipAddress));
	}
}