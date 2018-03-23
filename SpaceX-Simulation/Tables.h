#pragma once
#include <string>
#include <vector>
#include <sstream>

#include "LinkedList.h"

using namespace std;

const char ValueDeliminator = '|';
const char TupleDeliminator = '\n';
const string NullIndicator = "\\N";

string to_string(char* c, int size) {
	string result(c, size);
	if (size > 0 && c[0] == '\0') return NullIndicator;
	return result;
}
string to_string(char* c) {
	string result(c);
	if (c[0] == '\0') return NullIndicator;
	return result;
}
string to_string(char c) {
	string result = "";
	result.push_back(c);
	if (c == '\0') return NullIndicator;
	return result;
}
string V() {
	return to_string(ValueDeliminator);
}
string T() {
	return to_string(TupleDeliminator);
}

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
Dragon* createDragon(string SerialNumber, string Description) {
	Dragon d;
	for (int i = 0; i < 4; i++) {
		if (i < SerialNumber.size()) {
			d.SerialNumber[i] = SerialNumber[i];
		}
		else {
			d.SerialNumber[i] = '\0';
		}
	}
	for (int i = 0; i < 499; i++) {
		if (i < Description.size()) {
			d.Description[i] = Description[i];
		}
		else {
			d.Description[i] = '\0';
		}
	}
	d.Description[499] = '\0';
	Dragons.push_back(d);
	return Dragons.back();
}
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
LaunchSite* createLaunchSite(string Name, string Location) {
	LaunchSite l;
	for (int i = 0; i < 99; i++) {
		if (i < Name.size()) {
			l.Name[i] = Name[i];
		}
		else {
			l.Name[i] = '\0';
		}
	}
	l.Name[99] = '\0';
	for (int i = 0; i < 254; i++) {
		if (i < Location.size()) {
			l.Location[i] = Location[i];
		}
		else {
			l.Location[i] = '\0';
		}
	}
	l.Location[254] = '\0';
	LaunchSites.push_back(l);
	return LaunchSites.back();
}
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
Mission* createMission(int MissionNumber, string Title, string Description, Date date, LaunchSite* LaunchSiteName) {
	Mission m;
	m.MissionNumber = MissionNumber;
	for (int i = 0; i < 254; i++) {
		if (i < Title.size()) {
			m.Title[i] = Title[i];
		}
		else {
			m.Title[i] = '\0';
		}
	}
	m.Title[254] = '\0';
	for (int i = 0; i < 499; i++) {
		if (i < Description.size()) {
			m.Description[i] = Description[i];
		}
		else {
			m.Description[i] = '\0';
		}
	}
	m.Description[499] = '\0';
	m.date = date;
	m.LaunchSiteName = LaunchSiteName;
	Missions.push_back(m);
	return Missions.back();
}
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
flownBy* createFlownBy(Booster* BoosterID, Mission* MissionNumber, string LandingSite, string LandingOutcome) {
	flownBy f;
	f.BoosterID = BoosterID;
	f.MissionNumber = MissionNumber;
	for (int i = 0; i < 99; i++) {
		if (i < LandingSite.size()) {
			f.LandingSite[i] = LandingSite[i];
		}
		else {
			f.LandingSite[i] = '\0';
		}
	}
	f.LandingSite[99] = '\0';
	for (int i = 0; i < 499; i++) {
		if (i < LandingOutcome.size()) {
			f.LandingOutcome[i] = LandingOutcome[i];
		}
		else {
			f.LandingOutcome[i] = '\0';
		}
	}
	f.LandingOutcome[499] = '\0';
	flownBys.push_back(f);
	return flownBys.back();
}
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
Payload* createPayload(string Title, string Orbit, int PayloadMass, string Supplier, string MissionOutcome, Dragon* DragonSerial, Mission* MissionNumber) {
	Payload p;
	for (int i = 0; i < 99; i++) {
		if (i < Title.size()) {
			p.Title[i] = Title[i];
		}
		else {
			p.Title[i] = '\0';
		}
	}
	p.Title[99] = '\0';
	for (int i = 0; i < 99; i++) {
		if (i < Orbit.size()) {
			p.Orbit[i] = Orbit[i];
		}
		else {
			p.Orbit[i] = '\0';
		}
	}
	p.Orbit[99] = '\0';
	p.PayloadMass = PayloadMass;
	for (int i = 0; i < 254; i++) {
		if (i < Supplier.size()) {
			p.Supplier[i] = Supplier[i];
		}
		else {
			p.Supplier[i] = '\0';
		}
	}
	p.Supplier[254] = '\0';
	for (int i = 0; i < 499; i++) {
		if (i < MissionOutcome.size()) {
			p.MissionOutcome[i] = MissionOutcome[i];
		}
		else {
			p.MissionOutcome[i] = '\0';
		}
	}
	p.MissionOutcome[499] = '\0';
	p.DragonSerial = DragonSerial;
	p.MissionNumber = MissionNumber;
	Payloads.push_back(p);
	return Payloads.back();
}
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