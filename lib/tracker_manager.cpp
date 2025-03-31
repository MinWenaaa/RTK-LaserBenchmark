#include <cstring>
#include "tracker_manager.h"

using namespace LMF::Tracker;

void TrackerManager::ConnectTo(const char* ipAddress) {
	if (this->LMFTracker) this->LMFTracker->Disconnect();

	Connection^ con = gcnew Connection();
	if (strcmp(ipAddress, "0.0.0.0")) {
		this->LMFTracker = con->Connect(gcnew System::String(ipAddress));
	}
}