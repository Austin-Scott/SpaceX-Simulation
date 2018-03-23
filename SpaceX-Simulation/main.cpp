#include <iostream>
#include <fstream>

#include "Tables.h"
#include "RealData.h"
#include "SimulatedData.h"

using namespace std;

void writeResultsToFiles() {
	ofstream boosterFile("Booster.txt");
	if (boosterFile) {
		boosterFile << listBoosters();
	}
	boosterFile.close();

	ofstream flownByFile("flownBy.txt");
	if (flownByFile) {
		flownByFile << listflownBys();
	}
	flownByFile.close();

	ofstream MissionFile("Mission.txt");
	if (MissionFile) {
		MissionFile << listMissions();
	}
	MissionFile.close();

	ofstream PayloadFile("Payload.txt");
	if (PayloadFile) {
		PayloadFile << listPayloads();
	}
	PayloadFile.close();

	ofstream DragonFile("Dragon.txt");
	if (DragonFile) {
		DragonFile << listDragons();
	}
	DragonFile.close();

	ofstream LaunchSiteFile("LaunchSite.txt");
	if (LaunchSiteFile) {
		LaunchSiteFile << listLaunchSites();
	}
	LaunchSiteFile.close();
}

int main() {
	getRealData();
	runSimulation();
	writeResultsToFiles();
	cleanAllData();
}