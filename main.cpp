#include <iostream>

#include "tracker_manager.h"

int main() {
	TrackerManager::getInstance->ConnectTo("ATS600Simulator");
	int i;
	std::cin >> i;
	return 0;
}