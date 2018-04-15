#pragma once
#include <string>
#include <vector>
#include <sstream>

const char ValueDeliminator = '|';
const char TupleDeliminator = '\n';
const string NullIndicator = "\\N";

#include "Utility.h"

//Booster table
struct Booster {
	char BoosterID[5]; //Primary Key
	char FlightStatus[50];
	char CoreType[25];
	int BlockNumber;

	//Stored data not used in final database
	int flights;

	string insertTuple() {
		string result = "insert into " + getName() + " values ( '" + to_string(BoosterID, 5) + "', ";
		result = FlightStatus[0] == '\0' ? result + "NULL, " : result + "'" + sanitize(to_string(FlightStatus))+ "', ";
		result = CoreType[0] == '\0' ? result + "NULL, " : result + "'" + sanitize(to_string(CoreType)) + "', ";
		result = BlockNumber == INT_MIN ? result + "NULL" : result + to_string(BlockNumber);
		result += ")";
		return result;
	}
	static string createTable() {
		return "create table " + getName() + " ( BoosterID char(5) not null, FlightStatus varchar(50), CoreType varchar(25), BlockNumber int, primary key (BoosterID))";
	}
	static string getName() { return "Booster";  }

	friend Booster* createBooster(string BoosterID, string FlightStatus, string CoreType, int BlockNumber);
	static string getNextAvailableBoosterID() {
		string boosterID = "B";
		boosterID += padWithZeros(to_string(highestBoosterID+1), 4);
		return boosterID;
	}
private:
	static int highestBoosterID;
};
int Booster::highestBoosterID = 0;
LinkedList<Booster> Boosters;
Booster* createBooster(string BoosterID, string FlightStatus, string CoreType, int BlockNumber) {
	Booster b;
	strcpy(BoosterID, b.BoosterID, 5, false);
	strcpy(FlightStatus, b.FlightStatus, 50);
	strcpy(CoreType, b.CoreType, 25);
	b.BlockNumber = BlockNumber;
	b.flights = 0;

	int boosterNum = atoi(BoosterID.substr(1, 4).c_str());
	if (boosterNum > Booster::highestBoosterID) Booster::highestBoosterID = boosterNum;

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
		result += to_string(p->BoosterID, 5) + V() + to_string(p->FlightStatus) + V() + to_string(p->CoreType) + V() + blockNumber + T();
	}
	return result;
}
//End Booster table

//Dragon table
struct Dragon {
	char SerialNumber[4]; //Primary Key
	char Description[400];
	int BFS; //-1=null, 0=false, 1=true
	int FlightActive; //-1=null, 0=false, 1=true

	string insertTuple() {
		string result = "insert into " + getName() + " values ( '" + to_string(SerialNumber, 4) + "', ";
		result = Description[0] == '\0' ? result + "NULL, " : result + "'" + sanitize(to_string(Description)) + "', ";
		result += boolToString(BFS) + ", ";
		result += boolToString(FlightActive);
		result += ")";
		return result;
	}
	static string createTable() {
		return "create table " + getName() + " ( SerialNumber char(4) not null, Description varchar(400), BFS boolean, FlightActive boolean, primary key(SerialNumber) )";
	}
	static string getName() { return "Dragon";  }

	friend Dragon* createDragon(string SerialNumber, string Description, int BFS, int FlightActive);
	static string getNextAvailableSerialNumber() {
		string capsuleID = "C";
		capsuleID += padWithZeros(to_string(highestSerialNumber+1), 3);
		return capsuleID;
	}

private:
	static int highestSerialNumber;
};
int Dragon::highestSerialNumber = 0;
LinkedList<Dragon> Dragons;
Dragon* createDragon(string SerialNumber, string Description, int BFS, int FlightActive) {
	Dragon d;
	strcpy(SerialNumber, d.SerialNumber, 4, false);
	strcpy(Description, d.Description, 400);
	d.FlightActive = FlightActive;
	d.BFS = BFS;

	int capsuleNum = atoi(SerialNumber.substr(1, 3).c_str());
	if (capsuleNum > Dragon::highestSerialNumber) Dragon::highestSerialNumber = capsuleNum;

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
		result += to_string(p->SerialNumber, 4) + V() + to_string(p->Description) + V() + boolToListString(p->BFS) + V() + boolToListString(p->FlightActive) + T();
	}
	return result;
}
//End Dragon table

//LaunchSite table
struct LaunchSite {
	char SiteID[25]; //Primary Key
	char Name[255];
	char Location[255];
	int Active; //-1=null, 0=false, 1=true
	int BFR; //-1=null, 0=false, 1=true

	string insertTuple() {
		string result = "insert into " + getName() + " values ( '" + to_string(SiteID) + "', ";
		result = Name[0] == '\0' ? result + "NULL, " : result + "'" + sanitize(to_string(Name)) + "', ";
		result = Location[0] == '\0' ? result + "NULL, " : result + "'" + sanitize(to_string(Location)) + "', ";
		result += boolToString(Active) + ", ";
		result += boolToString(BFR);
		result += ")";
		return result;
	}
	static string createTable() {
		return "create table " + getName() + " (SiteID varchar(25) not null, Name varchar(255), Location varchar(255), Active boolean, BFR boolean, primary key(SiteID))";
	}
	static string getName() { return "LaunchSite";  }
};
LinkedList<LaunchSite> LaunchSites;
LaunchSite* createLaunchSite(string SiteID, string Name, string Location, int Active, int BFR) {
	LaunchSite l;
	strcpy(SiteID, l.SiteID, 25);
	strcpy(Name, l.Name, 255);
	strcpy(Location, l.Location, 255);
	l.Active = Active;
	l.BFR = BFR;
	LaunchSites.push_back(l);
	return LaunchSites.back();
}
LaunchSite* findLaunchSite(string SiteID) {
	for (auto i = LaunchSites.begin(); i.hasNext(); i.operator++()) {
		LaunchSite* p = &i;
		if (stringcmp(SiteID, p->SiteID, 25))
			return p;
	}
	return nullptr;
}
LaunchSite* findOrCreateLaunchSite(string SiteID, string Name, string Location, int Active, int BFR) {
	LaunchSite* result = findLaunchSite(SiteID);
	if (result == nullptr) {
		return createLaunchSite(SiteID,Name,Location,Active,BFR);
	}
	else {
		return result;
	}
}
string listLaunchSites() {
	string result = "";
	for (auto i = LaunchSites.begin(); i.hasNext(); i.operator++()) {
		auto p = &i;
		result += to_string(p->SiteID) + V() + to_string(p->Name) + V() + to_string(p->Location) + V() + boolToListString(p->Active)+ V() + boolToListString(p->BFR) + T();
	}
	return result;
}
//End LaunchSite table

//LandingSite table
struct LandingSite {
	char SiteID[25]; //Primary Key
	char Name[255];
	int BFR; //-1=null, 0=false, 1=true

	string insertTuple() {
		string result = "insert into " + getName() + " values ( '" + to_string(SiteID) + "', ";
		result = Name[0] == '\0' ? result + "NULL, " : result + "'" + sanitize(to_string(Name)) + "', ";
		result += boolToString(BFR);
		result += ")";
		return result;
	}
	static string createTable() {
		return "create table " + getName() + " (SiteID varchar(25) not null,Name varchar(255), BFR boolean, primary key(SiteID))";
	}
	static string getName() { return "LandingSite"; }
};
LinkedList<LandingSite> LandingSites;
LandingSite* createLandingSite(string SiteID, string Name, int BFR) {
	LandingSite l;
	strcpy(SiteID, l.SiteID, 25);
	strcpy(Name, l.Name, 255);
	l.BFR = BFR;
	LandingSites.push_back(l);
	return LandingSites.back();
}
LandingSite* findLandingSite(string SiteID) {
	for (auto i = LandingSites.begin(); i.hasNext(); i.operator++()) {
		LandingSite* p = &i;
		if (stringcmp(SiteID, p->SiteID, 25))
			return p;
	}
	return nullptr;
}
LandingSite* findOrCreateLandingSite(string SiteID, string Name, int BFR) {
	LandingSite* result = findLandingSite(SiteID);
	if (result == nullptr) {
		return createLandingSite(SiteID, Name, BFR);
	}
	else {
		return result;
	}
}
string listLandingSites() {
	string result = "";
	for (auto i = LandingSites.begin(); i.hasNext(); i.operator++()) {
		auto p = &i;
		result += to_string(p->SiteID) + V() + to_string(p->Name) + V() + boolToListString(p->BFR) + T();
	}
	return result;
}
//End LandingSite table

//Mission table
struct Mission {
	int MissionNumber; //Primary Key
	LaunchSite* LaunchSiteName;
	char Title[255];
	char Description[400];
	Date date;
	int LaunchSuccess; //-1=null, 0=false, 1=true

	string insertTuple() {
		string result = "insert into " + getName() + " values ( " + to_string(MissionNumber) + ", ";
		result = LaunchSiteName == nullptr ? result + "NULL, " : result + "'" + to_string(LaunchSiteName->Name) + "', ";
		result = Title[0] == '\0' ? result + "NULL, " : result + "'" + sanitize(to_string(Title)) + "', ";
		result = Description[0] == '\0' ? result + "NULL, " : result + "'" + sanitize(to_string(Description)) + "', ";
		result += "'" + to_string(date) + "', ";
		result += boolToString(LaunchSuccess);
		result += ")";
		return result;
	}
	static string createTable() {
		return "create table " + getName() + " (MissionNumber int not null, LaunchSite varchar(25) not null, Title varchar(255), Description varchar(400), Date date, LaunchSuccess boolean, primary key(MissionNumber),foreign key(LaunchSite) references LaunchSite(SiteID))";
	}
	static string getName() { return "Mission";  }

	friend Mission* createMission(int MissionNumber, LaunchSite* LaunchSiteName, string Title, string Description, Date date, int LaunchSuccess);
	static int getNextAvailableMissionNumber() {
		return highestMissionNumber + 1;
	}

private:
	static int highestMissionNumber;
};
int Mission::highestMissionNumber = 0;
LinkedList<Mission> Missions;
Mission* createMission(int MissionNumber, LaunchSite* LaunchSiteName, string Title, string Description, Date date, int LaunchSuccess) {
	Mission m;
	m.MissionNumber = MissionNumber;
	m.LaunchSiteName = LaunchSiteName;
	strcpy(Title, m.Title, 255);
	strcpy(Description, m.Description, 400);
	m.date = date;
	m.LaunchSuccess = LaunchSuccess;

	if (MissionNumber > Mission::highestMissionNumber) Mission::highestMissionNumber = MissionNumber;

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
		result += to_string(p->MissionNumber) + V() + to_string(p->LaunchSiteName->Name) + V() + to_string(p->Title) + V() + to_string(p->Description) + V() + to_string(p->date) + V() + boolToListString(p->LaunchSuccess) + T();
	}
	return result;
}
//End Mission Table

//flownBy table
struct flownBy {
	Booster* BoosterID; //Primary Key
	Mission* MissionNumber; //Primary Key
	LandingSite* LandSite; //Foreign Key
	char LandingOutcome[50];
	int LandingSuccess; //-1=null, 0=false, 1=true

	string insertTuple() {
		string result = "insert into " + getName() + " values ( '" + to_string(BoosterID->BoosterID, 5) + "', " + to_string(MissionNumber->MissionNumber) + ", ";
		result = LandSite == nullptr ? result + "NULL, " : result + "'" + sanitize(to_string(LandSite->SiteID)) + "', ";
		result = LandingOutcome[0] == '\0' ? result + "NULL, " : result + "'" + sanitize(to_string(LandingOutcome)) + "', ";
		result += boolToString(LandingSuccess);
		result += ")";
		return result;
	}
	static string createTable() {
		return "create table " + getName() + " (BoosterID char(5) not null,MissionNumber int not null,LandingSite varchar(25),LandingOutcome varchar(50), LandingSuccess boolean, primary key(BoosterID, MissionNumber),foreign key(BoosterID) references Booster(BoosterID),foreign key(MissionNumber) references Mission(MissionNumber), foreign key(LandingSite) references LandingSite(SiteID))";
	}
	static string getName() { return "flownBy";  }
};
LinkedList<flownBy> flownBys;
flownBy* createFlownBy(Booster* BoosterID, Mission* MissionNumber, LandingSite* LandingSite, string LandingOutcome, int LandingSuccess) {
	flownBy f;
	f.BoosterID = BoosterID;
	f.MissionNumber = MissionNumber;
	f.LandSite = LandingSite;
	strcpy(LandingOutcome, f.LandingOutcome, 50);
	f.LandingSuccess = LandingSuccess;
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
		string landSite = p->LandSite == nullptr ? NullIndicator : to_string(p->LandSite->SiteID);
		result += to_string(p->BoosterID->BoosterID, 5) + V() + to_string(p->MissionNumber->MissionNumber) + V() + landSite + V() + to_string(p->LandingOutcome) + V() + boolToListString(p->LandingSuccess) + T();
	}
	return result;
}
//End flownBy table

//Payload table
struct Payload {
	char Title[100]; //Primary Key
	Mission* MissionNumber; //Primary Key
	LaunchSite* DestinationSite; //Foreign Key
	Dragon* DragonSerial; //Foreign Key
	char Orbit[50];
	int PayloadMass;
	char Supplier[50];
	char MissionOutcome[50];
	int CrewMembers;

	string insertTuple() {
		string result = "insert into " + getName() + " values ( '" + sanitize(to_string(Title)) + "', ";
		result += to_string(MissionNumber->MissionNumber) + ", ";
		result = DestinationSite == nullptr ? result + "NULL, " : result + "'" + sanitize(to_string(DestinationSite->SiteID)) + "', ";
		result = DragonSerial == nullptr ? result + "NULL, " : result + "'" + to_string(DragonSerial->SerialNumber, 4) + "', ";
		result = Orbit[0] == '\0' ? result + "NULL, " : result + "'" + sanitize(to_string(Orbit)) + "', ";
		result = PayloadMass == INT_MIN ? result + "NULL, " : result + to_string(PayloadMass) + ", ";
		result = Supplier[0] == '\0' ? result + "NULL, " : result + "'" + sanitize(to_string(Supplier)) + "', ";
		result = MissionOutcome[0] == '\0' ? result + "NULL, " : result + "'" + sanitize(to_string(MissionOutcome)) + "', ";
		result += to_string(CrewMembers);
		result += ")";
		return result;
	}
	static string createTable() {
		return "create table " + getName() + " (Title varchar(100) not null, MissionNumber int not null, DestinationSite varchar(25), DragonSerial char(4), Orbit varchar(50),PayloadMass int,Supplier varchar(50),MissionOutcome varchar(50), CrewMembers int, primary key(Title, MissionNumber), foreign key(DestinationSite) references LaunchSite(SiteID), foreign key(DragonSerial) references Dragon(SerialNumber),foreign key(MissionNumber) references Mission(MissionNumber))";
	}
	static string getName() { return "Payload";  }
};
LinkedList<Payload> Payloads;
Payload* createPayload(string Title, Mission* MissionNumber, LaunchSite* DestinationSite, Dragon* DragonSerial, string Orbit, int PayloadMass, string Supplier, string MissionOutcome, int CrewMembers) {
	Payload p;
	strcpy(Title, p.Title, 100);
	p.MissionNumber = MissionNumber;
	p.DestinationSite = DestinationSite;
	p.DragonSerial = DragonSerial;
	strcpy(Orbit, p.Orbit, 50);
	p.PayloadMass = PayloadMass;
	strcpy(Supplier, p.Supplier, 50);
	strcpy(MissionOutcome, p.MissionOutcome, 50);
	p.CrewMembers = CrewMembers;
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
		string destinationSite = p->DestinationSite == nullptr ? NullIndicator : to_string(p->DestinationSite->SiteID);
		result += to_string(p->Title) + V() + to_string(p->MissionNumber->MissionNumber) + V() + destinationSite + V() + dragonSerial + V() + to_string(p->Orbit) + V() + payloadMass + V() + to_string(p->Supplier) + V() + to_string(p->MissionOutcome) + V() + to_string(p->CrewMembers) + T();
	}
	return result;
}
//End Payload table

//Functions involving all tables

void cleanAllData() {
	Boosters.deleteAll();
	flownBys.deleteAll();
	Missions.deleteAll();
	Payloads.deleteAll();
	Dragons.deleteAll();
	LaunchSites.deleteAll();
	LandingSites.deleteAll();
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

	ofstream LandingSiteFile("output/LandingSite.txt", ios::out);
	if (LandingSiteFile) {
		LandingSiteFile << listLandingSites();
	}
	LandingSiteFile.close();

	cout << "Done." << endl;
}

void updateDatabase(string address, string schema, string username, string password, bool soft) {
	cout << "Connecting to database at " << address << "..." << endl;
	connectToDatabase(address, schema, username, password);

	if (isSQLConnected()) {

		if (soft) {
			cout << "Dropping all tuples from tables..." << endl;
			//drop all tuples
			executeSQL(dropTuples(flownBy::getName()));
			executeSQL(dropTuples(Payload::getName()));
			executeSQL(dropTuples(Booster::getName()));
			executeSQL(dropTuples(Dragon::getName()));
			executeSQL(dropTuples(Mission::getName()));
			executeSQL(dropTuples(LaunchSite::getName()));
			executeSQL(dropTuples(LandingSite::getName()));
		}
		else {

			cout << "Dropping any pre-existing tables..." << endl;
			//Drop tables if they exist
			executeSQL(dropTable(flownBy::getName()));
			executeSQL(dropTable(Payload::getName()));
			executeSQL(dropTable(Booster::getName()));
			executeSQL(dropTable(Dragon::getName()));
			executeSQL(dropTable(Mission::getName()));
			executeSQL(dropTable(LaunchSite::getName()));
			executeSQL(dropTable(LandingSite::getName()));

			cout << "Creating tables..." << endl;
			//Re-create the dropped tables
			executeSQL(Booster::createTable());
			executeSQL(Dragon::createTable());
			executeSQL(LandingSite::createTable());
			executeSQL(LaunchSite::createTable());
			executeSQL(Mission::createTable());
			executeSQL(flownBy::createTable());
			executeSQL(Payload::createTable());

		}

		//Populate the database
		cout << "Inserting tuples into " << Booster::getName() << "..." << endl;
		for (auto iter = Boosters.begin(); iter.hasNext(); iter.operator++()) {
			executeSQL((&iter)->insertTuple());
		}
		cout << "Inserting tuples into " << Dragon::getName() << "..." << endl;
		for (auto iter = Dragons.begin(); iter.hasNext(); iter.operator++()) {
			executeSQL((&iter)->insertTuple());
		}
		cout << "Inserting tuples into " << LaunchSite::getName() << "..." << endl;
		for (auto iter = LaunchSites.begin(); iter.hasNext(); iter.operator++()) {
			executeSQL((&iter)->insertTuple());
		}
		cout << "Inserting tuples into " << LandingSite::getName() << "..." << endl;
		for (auto iter = LandingSites.begin(); iter.hasNext(); iter.operator++()) {
			executeSQL((&iter)->insertTuple());
		}
		cout << "Inserting tuples into " << Mission::getName() << "..." << endl;
		for (auto iter = Missions.begin(); iter.hasNext(); iter.operator++()) {
			executeSQL((&iter)->insertTuple());
		}
		cout << "Inserting tuples into " << flownBy::getName() << "..." << endl;
		for (auto iter = flownBys.begin(); iter.hasNext(); iter.operator++()) {
			executeSQL((&iter)->insertTuple());
		}
		cout << "Inserting tuples into " << Payload::getName() << "..." << endl;
		for (auto iter = Payloads.begin(); iter.hasNext(); iter.operator++()) {
			executeSQL((&iter)->insertTuple());
		}

		cout << "Complete. Disconnecting from database." << endl;

		disconnectFromDatabase();

	}
	else {
		cout << "Could not connect to database. Moving on." << endl;
	}
}