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
void strcpy(string src, char* dest, int n, bool varlength=true) {
	int end = varlength ? n - 1 : n;
	for (int i = 0; i < end; i++) {
		if (i < src.size()) {
			dest[i] = src[i];
		}
		else {
			dest[i] = '\0';
		}
	}
	if(varlength)
		dest[n - 1] = '\0';
}
void strmov(const char* src, char* dest, int n) {
	for (int i = 0; i < n; i++) {
		dest[i] = src[i];
	}
}
bool stringcmp(string a, char* b, int n, bool varlength = true) {
	for (int i = 0; i < n; i++) {
		if (varlength&&(i >= a.size() || i == n - 1) && b[i] == '\0') return true;
		if (!varlength&&i == n - 1 && a.size() == n&&a[i] == b[i]) return true;
		if (i >= a.size() || a[i] != b[i]) return false;
	}
	return false;
}

struct Booster {
	char BoosterID[5]; //Primary Key
	char FlightStatus[255];
	int BlockNumber;

	//Stored data not used in final database
	int flights;

	Booster() {}
	Booster(const Booster &other) {
		operator=(other);
	}
	void operator=(const Booster &other) {
		strmov(other.BoosterID, BoosterID, 5);
		strmov(other.FlightStatus, FlightStatus, 255);
		BlockNumber = other.BlockNumber;
		flights = other.flights;
	}
};
LinkedList<Booster> Boosters;
Booster* createBooster(string BoosterID, string FlightStatus, int BlockNumber) {
	Booster b;
	strcpy(BoosterID, b.BoosterID, 5, false);
	strcpy(FlightStatus, b.FlightStatus, 255);
	b.BlockNumber = BlockNumber;
	b.flights = 0;
	Boosters.push_back(b);
	return Boosters.back();
}
Booster* findBooster(string BoosterID) {
	for (auto i = Boosters.begin(); i.hasNext(); i.operator++()) {
		Booster* p = &i;
		if (stringcmp(BoosterID, p->BoosterID, 5, false))
			return p;
	}
	return nullptr;
}
string listBoosters() {
	string result = "";
	for (auto i = Boosters.begin(); i.hasNext(); i.operator++()) {
		auto p = &i;
		string blockNumber = p->BlockNumber != INT_MIN ? to_string(p->BlockNumber) : NullIndicator;
		result += to_string(p->BoosterID, 5) + V() + to_string(p->FlightStatus) + V() + blockNumber + T();
	}
	return result;
}

struct Dragon {
	char SerialNumber[4]; //Primary Key
	char Description[500];

	Dragon() {}
	Dragon(const Dragon &other) {
		operator=(other);
	}
	void operator=(const Dragon &other) {
		strmov(other.SerialNumber, SerialNumber, 4);
		strmov(other.Description, Description, 500);
	}
};
LinkedList<Dragon> Dragons;
Dragon* createDragon(string SerialNumber, string Description) {
	Dragon d;
	strcpy(SerialNumber, d.SerialNumber, 4, false);
	strcpy(Description, d.Description, 500);
	Dragons.push_back(d);
	return Dragons.back();
}
Dragon* findDragon(string SerialNumber) {
	for (auto i = Dragons.begin(); i.hasNext(); i.operator++()) {
		Dragon* p = &i;
		if (stringcmp(SerialNumber, p->SerialNumber, 4, false))
			return p;
	}
	return nullptr;
}
string listDragons() {
	string result = "";
	for (auto i = Dragons.begin(); i.hasNext(); i.operator++()) {
		auto p = &i;
		result += to_string(p->SerialNumber, 4) + V() + to_string(p->Description) + T();
	}
	return result;
}

struct LaunchSite {
	char Name[100]; //Primary Key
	char Location[255];

	LaunchSite() {}
	LaunchSite(const LaunchSite &other) {
		operator=(other);
	}
	void operator=(const LaunchSite &other) {
		strmov(other.Name, Name, 100);
		strmov(other.Location, Location, 255);
	}
};
LinkedList<LaunchSite> LaunchSites;
LaunchSite* createLaunchSite(string Name, string Location) {
	LaunchSite l;
	strcpy(Name, l.Name, 100);
	strcpy(Location, l.Location, 255);
	LaunchSites.push_back(l);
	return LaunchSites.back();
}
LaunchSite* findLaunchSite(string Name) {
	for (auto i = LaunchSites.begin(); i.hasNext(); i.operator++()) {
		LaunchSite* p = &i;
		if (stringcmp(Name, p->Name, 100))
			return p;
	}
	return nullptr;
}
LaunchSite* findOrCreateLaunchSite(string name, string location) {
	LaunchSite* result = findLaunchSite(name);
	if (result == nullptr) {
		return createLaunchSite(name, location);
	}
	else {
		return result;
	}
}
string listLaunchSites() {
	string result = "";
	for (auto i = LaunchSites.begin(); i.hasNext(); i.operator++()) {
		auto p = &i;
		result += to_string(p->Name) + V() + to_string(p->Location) + T();
	}
	return result;
}

int getDaysInMonth(int month, int year) {
	if (month != 2) {
		if (month < 8) {
			if (month % 2 == 0) {
				return 30;
			}
			else {
				return 31;
			}
		}
		else {
			if (month % 2 == 0) {
				return 31;
			}
			else {
				return 30;
			}
		}
	}
	else {
		if (year % 4 == 0 && (year % 100 != 0 || (year % 100 == 0 && year % 400 == 0))) { //If it is a leap year
			return 29;
		}
		else {
			return 28;
		}
	}
}
struct Date { //Meta-Structure for holding dates
	int day; 
	int month;
	int year; //Such as: 2018
	Date() {}
	Date(string date) { //Format YYYY-MM-DD
		if (date.size() == 10) {
			year = atoi(date.substr(0, 4).c_str());
			month = atoi(date.substr(5, 2).c_str());
			day = atoi(date.substr(8, 2).c_str());
		}
		else {
			year = 0;
			month = 0;
			day = 0;
		}
	}
	Date(int year, int month, int day) {
		this->year = year;
		this->month = month;
		this->day = day;
	}
	Date operator+(int days) { //Advance time by "days" days
		Date result = *this;
		for (int i = 0; i < days; i++) {
			result.day++;
			if (day > getDaysInMonth(result.month, result.year)) {
				result.day = 1;
				result.month++;
				if (result.month > 12) {
					result.month = 1;
					result.year++;
				}
			}
		}
		return result;
	}
	void operator+=(int days) {
		Date result = operator+(days);
		day = result.day;
		month = result.month;
		year = result.year;
	}
	bool operator==(Date other) {
		return year == other.year&&month == other.month&&day == other.day;
	}
	bool operator!=(Date other) {
		return !operator==(other);
	}
	bool operator>(Date other) {
		if (year > other.year) {
			return true;
		}
		else if (year < other.year) {
			return false;
		}
		else {
			if (month > other.month) {
				return true;
			}
			else if (month < other.month) {
				return false;
			}
			else {
				if (day > other.day) {
					return true;
				}
				else if (day < other.day) {
					return false;
				}
				else {
					return false;
				}
			}
		}
	}
	int operator-(Date other) { //Get number of days between this date and other date
		int result = 0;
		if (operator>(other)) {
			while (other != *this) {
				result++;
				other += 1;
			}
		}
		else {
			Date temp = *this;
			while (temp != other) {
				result++;
				temp += 1;
			}
		}
		return result;
	}
};
string padWithZeros(string str, int width) {
	if (str.size() < width) {
		int zeros = width - str.size();
		for (int i = 0; i < zeros; i++) {
			str = '0' + str;
		}
	}
	return str;
}
string to_string(Date date) {
	string result;
	string sYear = to_string(date.year);
	sYear = padWithZeros(sYear, 4);
	string sMonth = to_string(date.month);
	sMonth = padWithZeros(sMonth, 2);
	string sDay = to_string(date.day);
	sDay = padWithZeros(sDay, 2);
	result = sYear + "-" + sMonth + "-" + sDay;
	return result;
}

struct Mission {
	int MissionNumber; //Primary Key
	char Title[255];
	char Description[500];
	Date date;
	LaunchSite* LaunchSiteName;

	Mission() {}
	Mission(const Mission &other) {
		operator=(other);
	}
	void operator=(const Mission &other) {
		MissionNumber = other.MissionNumber;
		strmov(other.Title, Title, 255);
		strmov(other.Description, Description, 500);
		date = other.date;
		LaunchSiteName = other.LaunchSiteName;
	}
};
LinkedList<Mission> Missions;
Mission* createMission(int MissionNumber, string Title, string Description, Date date, LaunchSite* LaunchSiteName) {
	Mission m;
	m.MissionNumber = MissionNumber;
	strcpy(Title, m.Title, 255);
	strcpy(Description, m.Description, 500);
	m.date = date;
	m.LaunchSiteName = LaunchSiteName;
	Missions.push_back(m);
	return Missions.back();
}
Mission* findMission(int MissionNumber) {
	for (auto i = Missions.begin(); i.hasNext(); i.operator++()) {
		Mission* p = &i;
		if (p->MissionNumber == MissionNumber) return p;
	}
	return nullptr;
}
string listMissions() {
	string result = "";
	for (auto i = Missions.begin(); i.hasNext(); i.operator++()) {
		auto p = &i;
		result += to_string(p->MissionNumber) + V() + to_string(p->Title) + V() + to_string(p->Description) + V() + to_string(p->date) + V() + to_string(p->LaunchSiteName->Name) + T();
	}
	return result;
}

struct flownBy {
	Booster* BoosterID; //Primary Key
	Mission* MissionNumber; //Primary Key
	char LandingSite[100];
	char LandingOutcome[500];

	flownBy() {}
	flownBy(const flownBy &other) {
		operator=(other);
	}
	void operator=(const flownBy &other) {
		BoosterID = other.BoosterID;
		MissionNumber = other.MissionNumber;
		strmov(other.LandingSite, LandingSite, 100);
		strmov(other.LandingOutcome, LandingOutcome, 500);
	}
};
LinkedList<flownBy> flownBys;
flownBy* createFlownBy(Booster* BoosterID, Mission* MissionNumber, string LandingSite, string LandingOutcome) {
	flownBy f;
	f.BoosterID = BoosterID;
	f.MissionNumber = MissionNumber;
	strcpy(LandingSite, f.LandingSite, 100);
	strcpy(LandingOutcome, f.LandingOutcome, 500);
	flownBys.push_back(f);
	return flownBys.back();
}
flownBy* findFlownBy(Booster* BoosterID, Mission* MissionNumber) {
	for (auto i = flownBys.begin(); i.hasNext(); i.operator++()) {
		flownBy* p = &i;
		if (p->BoosterID == BoosterID&&p->MissionNumber == MissionNumber)
			return p;
	}
	return nullptr;
}
string listflownBys() {
	string result = "";
	for (auto i = flownBys.begin(); i.hasNext(); i.operator++()) {
		auto p = &i;
		result += to_string(p->BoosterID->BoosterID, 5) + V() + to_string(p->MissionNumber->MissionNumber) + V() + to_string(p->LandingSite) + V() + to_string(p->LandingOutcome) + T();
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

	Payload() {}
	Payload(const Payload &other) {
		operator=(other);
	}
	void operator=(const Payload &other) {
		strmov(other.Title, Title, 100);
		strmov(other.Orbit, Orbit, 100);
		PayloadMass = other.PayloadMass;
		strmov(other.Supplier, Supplier, 255);
		strmov(other.MissionOutcome, MissionOutcome, 500);
		DragonSerial = other.DragonSerial;
		MissionNumber = other.MissionNumber;
	}
};
LinkedList<Payload> Payloads;
Payload* createPayload(string Title, string Orbit, int PayloadMass, string Supplier, string MissionOutcome, Dragon* DragonSerial, Mission* MissionNumber) {
	Payload p;
	strcpy(Title, p.Title, 100);
	strcpy(Orbit, p.Orbit, 100);
	p.PayloadMass = PayloadMass;
	strcpy(Supplier, p.Supplier, 255);
	strcpy(MissionOutcome, p.MissionOutcome, 500);
	p.DragonSerial = DragonSerial;
	p.MissionNumber = MissionNumber;
	Payloads.push_back(p);
	return Payloads.back();
}
Payload* findPayload(string Title) {
	for (auto i = Payloads.begin(); i.hasNext(); i.operator++()) {
		Payload* p = &i;
		if (stringcmp(Title, p->Title, 100))
			return p;
	}
	return nullptr;
}
string listPayloads() {
	string result = "";
	for (auto i = Payloads.begin(); i.hasNext(); i.operator++()) {
		auto p = &i;
		string payloadMass = p->PayloadMass != INT_MIN ? to_string(p->PayloadMass) : NullIndicator;
		string dragonSerial = p->DragonSerial != nullptr ? to_string(p->DragonSerial->SerialNumber, 4) : NullIndicator;
		result += to_string(p->Title) + V() + to_string(p->Orbit) + V() + payloadMass + V() + to_string(p->Supplier) + V() + to_string(p->MissionOutcome) + V()
			+ dragonSerial + V() + to_string(p->MissionNumber->MissionNumber) + T();
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

void writeResultsToFiles() {
	cout << "Outputing results to files..." << endl;

	ofstream boosterFile("output/Booster.txt", ios::out);
	if (boosterFile) {
		boosterFile << listBoosters();
	}
	boosterFile.close();

	ofstream flownByFile("output/flownBy.txt", ios::out);
	if (flownByFile) {
		flownByFile << listflownBys();
	}
	flownByFile.close();

	ofstream MissionFile("output/Mission.txt", ios::out);
	if (MissionFile) {
		MissionFile << listMissions();
	}
	MissionFile.close();

	ofstream PayloadFile("output/Payload.txt", ios::out);
	if (PayloadFile) {
		PayloadFile << listPayloads();
	}
	PayloadFile.close();

	ofstream DragonFile("output/Dragon.txt", ios::out);
	if (DragonFile) {
		DragonFile << listDragons();
	}
	DragonFile.close();

	ofstream LaunchSiteFile("output/LaunchSite.txt", ios::out);
	if (LaunchSiteFile) {
		LaunchSiteFile << listLaunchSites();
	}
	LaunchSiteFile.close();

	cout << "Done." << endl;
}