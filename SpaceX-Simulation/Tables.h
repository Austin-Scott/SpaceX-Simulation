#pragma once
#include <string>
#include <vector>
#include <sstream>

#include "LinkedList.h"

using namespace std;

const char ValueDeliminator = '|';
const char TupleDeliminator = '\n';

string to_string(char* c, int size) {
	string result(c, size);
	return result;
}
string to_string(char* c) {
	string result(c);
	return result;
}
string to_string(char c) {
	string result = "";
	result.push_back(c);
	return result;
}
string V() {
	return to_string(ValueDeliminator);
}
string T() {
	return to_string(TupleDeliminator);
}

//TODO: Add "creator" methods for all structors below

struct Booster {
	char BoosterID[5]; //Primary Key
	char FlightStatus[255];
	int BlockNumber;
};
LinkedList<Booster> Boosters;
Booster* findBooster(string BoosterID) {
	for (auto i = Boosters.begin(); i.hasNext(); i++) {
		Booster* p = &i;
		for (int j = 0; j < 100; j++) {
			if (j >= BoosterID.size()) break;
			if (BoosterID[j] != p->BoosterID[j]) break;
			if (j == 4) return p;
		}
	}
	return nullptr;
}
string listBoosters() {
	string result = "";
	for (auto i = Boosters.begin(); i.hasNext(); i++) {
		auto p = &i;
		result += to_string(p->BoosterID, 5) + V() + to_string(p->flightStatus) + V() + to_string(p->BlockNumber) + T();
	}
	return result;
}

struct Dragon {
	char SerialNumber[4]; //Primary Key
	char Description[500];
};
LinkedList<Dragon> Dragons;
Dragon* findDragon(string SerialNumber) {
	for (auto i = Dragons.begin(); i.hasNext(); i++) {
		Dragon* p = &i;
		for (int j = 0; j < 100; j++) {
			if (j >= SerialNumber.size()) break;
			if (SerialNumber[j] != p->SerialNumber[j]) break;
			if (j == 3) return p;
		}
	}
	return nullptr;
}
string listDragons() {
	string result = "";
	for (auto i = Dragons.begin(); i.hasNext(); i++) {
		auto p = &i;
		result += to_string(p->SerialNumber, 4) + V() + to_string(p->Description) + T();
	}
	return result;
}

struct LaunchSite {
	char Name[100]; //Primary Key
	char Location[255];
};
LinkedList<LaunchSite> LaunchSites;
LaunchSite* findLaunchSite(string Name) {
	for (auto i = LaunchSites.begin(); i.hasNext(); i++) {
		LaunchSite* p = &i;
		for (int j = 0; j < 100; j++) {
			if (j >= Name.size()) break;
			if (p->Name[j] == '\0') return p;
			if (Name[j] != p->Name[j]) break;
			if (j == 99) return p;
		}
	}
	return nullptr;
}
string listLaunchSites() {
	string result = "";
	for (auto i = LaunchSites.begin(); i.hasNext(); i++) {
		auto p = &i;
		result += to_string(p->Name) + V() + to_string(p->Location) + T();
	}
	return result;
}

struct Date { //Meta-Structure for holding dates
	int day; 
	int month;
	int year; //Such as: 2018
};
string to_string(Date date) {
	string result;
	//TODO: Add date string convertion
	return result;
}

struct Mission {
	int MissionNumber; //Primary Key
	char Title[255];
	char Description[500];
	Date date;
	LaunchSite* LaunchSiteName;
};
LinkedList<Mission> Missions;
Mission* findMission(int MissionNumber) {
	for (auto i = Missions.begin(); i.hasNext(); i++) {
		Mission* p = &i;
		if (p->MissionNumber == MissionNumber) return p;
	}
	return nullptr;
}
string listMissions() {
	string result = "";
	for (auto i = Missions.begin(); i.hasNext(); i++) {
		auto p = &i;
		result += to_string(p->MissionNumber) + V() + to_string(p->Title) + V() + to_string(p->Description) + V() + to_string(p->date) + V() + to_string(findLaunchSite(p->LaunchSiteName)->Name) + T();
	}
	return result;
}

struct flownBy {
	Booster* BoosterID; //Primary Key
	Mission* MissionNumber; //Primary Key
	char LandingSite[100];
	char LandingOutcome[500];
};
LinkedList<flownBy> flownBys;
flownBy* findFlownBy(Booster* BoosterID, Mission* MissionNumber) {
	for (auto i = flownBys.begin(); i.hasNext(); i++) {
		flownBy* p = &i;
		if (p->BoosterID == BoosterID&&p->MissionNumber == MissionNumber)
			return p;
	}
	return nullptr;
}
string listflownBys() {
	string result = "";
	for (auto i = flownBys.begin(); i.hasNext(); i++) {
		auto p = &i;
		result += to_string(findBooster(p->BoosterID)->BoosterID) + V() + to_string(findMission(p->MissionNumber)->MissionNumber) + V() + to_string(p->LandingSite) + V() + to_string(p->LandingOutcome) + T();
	}
	return result;
}

struct Payload {
	char Title[100]; //Primary Key
	char Orbit[100];
	int PayloadMass;
	char Supplier[255];
	char MissionOutcome[500];
	Dragon* DragonSerial;
	Mission* MissionNumber; //Must not be null
};
LinkedList<Payload> Payloads;
Payload* findPayload(string Title) {
	for (auto i = Payloads.begin(); i.hasNext(); i++) {
		Payload* p = &i;
		for (int j = 0; j < 100; j++) {
			if (j >= Title.size()) break;
			if (p->Title[j] == '\0') return p;
			if (Title[j] != p->Title[j]) break;
			if (j == 99) return p;
		}
	}
	return nullptr;
}
string listPayloads() {
	string result = "";
	for (auto i = Payloads.begin(); i.hasNext(); i++) {
		auto p = &i;
		result += to_string(p->Title) + V() + to_string(p->Orbit) + V() + to_string(p->PayloadMass) + V() + to_string(p->Supplier) + V() + to_string(p->MissionOutcome) + V()
			+ to_string(findDragon(p->DragonSerial)->SerialNumber) + V() + to_string(findMission(p->MissionNumber)->MissionNumber) + T();
	}
	return result;
}
void cleanAllData() {
	Boosters.deleteAll();
	flownBys.deleteAll();
	Missions.deleteAll();
	Payloads.deleteAll();
	Dragons.deleteAll();
	LaunchSites.deleteAll();
}