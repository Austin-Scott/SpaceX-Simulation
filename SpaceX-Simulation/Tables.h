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
	char FlightStatus[255];
	int BlockNumber;

	//Stored data not used in final database
	int flights;

	string insertTuple() {
		string result = "insert into " + getName() + " values ( '" + to_string(BoosterID, 5) + "', ";
		result = FlightStatus[0] == '\0' ? result + "NULL, " : result + "'" + sanitize(to_string(FlightStatus))+ "', ";
		result = BlockNumber == INT_MIN ? result + "NULL" : result + to_string(BlockNumber);
		result += ")";
		return result;
	}
	static string createTable() {
		return "create table " + getName() + " ( BoosterID char(5) not null, FlightStatus varchar(255), BlockNumber int, primary key (BoosterID))";
	}
	static string getName() { return "Booster";  }
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
//End Booster table

//Dragon table
struct Dragon {
	char SerialNumber[4]; //Primary Key
	char Description[500];
	int FlightActive; //-1=null, 0=false, 1=true

	virtual string insertTuple() {
		string result = "insert into " + getName() + " values ( '" + to_string(SerialNumber, 4) + "', ";
		result = Description[0] == '\0' ? result + "NULL, " : result + "'" + sanitize(to_string(Description)) + "', ";
		result += boolToString(FlightActive);
		result += ")";
		return result;
	}
	static string createTable() {
		return "create table " + getName() + " ( SerialNumber char(4) not null, Description varchar(500), FlightActive boolean, primary key(SerialNumber) )";
	}
	static string getName() { return "Dragon";  }
};
LinkedList<Dragon> Dragons;
Dragon* createDragon(string SerialNumber, string Description, int FlightActive) {
	Dragon d;
	strcpy(SerialNumber, d.SerialNumber, 4, false);
	strcpy(Description, d.Description, 500);
	d.FlightActive = FlightActive;
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
		result += to_string(p->SerialNumber, 4) + V() + to_string(p->Description) + V() + boolToListString(p->FlightActive) + T();
	}
	return result;
}
//End Dragon table

//LaunchSite table
struct LaunchSite {
	char Name[100]; //Primary Key
	char Location[255];

	virtual string insertTuple() {
		string result = "insert into " + getName() + " values ( '" + to_string(Name) + "', ";
		result = Location[0] == '\0' ? result + "NULL" : result + "'" + sanitize(to_string(Location)) + "'";
		result += ")";
		return result;
	}
	static string createTable() {
		return "create table " + getName() + " (Name varchar(100) not null,Location varchar(255),primary key(Name))";
	}
	static string getName() { return "LaunchSite";  }
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
//End LaunchSite table

//Mission table
struct Mission {
	int MissionNumber; //Primary Key
	char Title[255];
	char Description[500];
	Date date;
	LaunchSite* LaunchSiteName;
	int LaunchSuccess; //-1=null, 0=false, 1=true

	virtual string insertTuple() {
		string result = "insert into " + getName() + " values ( " + to_string(MissionNumber) + ", ";
		result = Title[0] == '\0' ? result + "NULL, " : result + "'" + sanitize(to_string(Title)) + "', ";
		result = Description[0] == '\0' ? result + "NULL, " : result + "'" + sanitize(to_string(Description)) + "', ";
		result += "'" + to_string(date) + "', ";
		result = LaunchSiteName == nullptr ? result + "NULL, " : result + "'" + to_string(LaunchSiteName->Name) + "', ";
		result += boolToString(LaunchSuccess);
		result += ")";
		return result;
	}
	static string createTable() {
		return "create table " + getName() + " (MissionNumber int not null,Title varchar(255),Description varchar(500),Date date,LaunchSiteName varchar(100) not null, LaunchSuccess boolean, primary key(MissionNumber),foreign key(LaunchSiteName) references LaunchSite(Name))";
	}
	static string getName() { return "Mission";  }
};
LinkedList<Mission> Missions;
Mission* createMission(int MissionNumber, string Title, string Description, Date date, LaunchSite* LaunchSiteName, int LaunchSuccess) {
	Mission m;
	m.MissionNumber = MissionNumber;
	strcpy(Title, m.Title, 255);
	strcpy(Description, m.Description, 500);
	m.date = date;
	m.LaunchSiteName = LaunchSiteName;
	m.LaunchSuccess = LaunchSuccess;
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
		result += to_string(p->MissionNumber) + V() + to_string(p->Title) + V() + to_string(p->Description) + V() + to_string(p->date) + V() + to_string(p->LaunchSiteName->Name) + V() + boolToListString(p->LaunchSuccess) + T();
	}
	return result;
}
//End Mission Table

//flownBy table
struct flownBy {
	Booster* BoosterID; //Primary Key
	Mission* MissionNumber; //Primary Key
	char LandingSite[100];
	char LandingOutcome[500];
	int LandingSuccess; //-1=null, 0=false, 1=true

	virtual string insertTuple() {
		string result = "insert into " + getName() + " values ( '" + to_string(BoosterID->BoosterID, 5) + "', " + to_string(MissionNumber->MissionNumber) + ", ";
		result = LandingSite[0] == '\0' ? result + "NULL, " : result + "'" + sanitize(to_string(LandingSite)) + "', ";
		result = LandingOutcome[0] == '\0' ? result + "NULL, " : result + "'" + sanitize(to_string(LandingOutcome)) + "', ";
		result += boolToString(LandingSuccess);
		result += ")";
		return result;
	}
	static string createTable() {
		return "create table " + getName() + " (BoosterID char(5) not null,MissionNumber int not null,LandingSite varchar(100),LandingOutcome varchar(500), LandingSuccess boolean, primary key(BoosterID, MissionNumber),foreign key(BoosterID) references Booster(BoosterID),foreign key(MissionNumber) references Mission(MissionNumber))";
	}
	static string getName() { return "flownBy";  }
};
LinkedList<flownBy> flownBys;
flownBy* createFlownBy(Booster* BoosterID, Mission* MissionNumber, string LandingSite, string LandingOutcome, int LandingSuccess) {
	flownBy f;
	f.BoosterID = BoosterID;
	f.MissionNumber = MissionNumber;
	strcpy(LandingSite, f.LandingSite, 100);
	strcpy(LandingOutcome, f.LandingOutcome, 500);
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
		result += to_string(p->BoosterID->BoosterID, 5) + V() + to_string(p->MissionNumber->MissionNumber) + V() + to_string(p->LandingSite) + V() + to_string(p->LandingOutcome) + V() + boolToListString(p->LandingSuccess) + T();
	}
	return result;
}
//End flownBy table

//Payload table
struct Payload {
	char Title[100]; //Primary Key
	char Orbit[100];
	int PayloadMass;
	char Supplier[255];
	char MissionOutcome[500];
	Dragon* DragonSerial;
	Mission* MissionNumber; //Must not be null
	int CrewMembers;

	virtual string insertTuple() {
		string result = "insert into " + getName() + " values ( '" + sanitize(to_string(Title)) + "', ";
		result = Orbit[0] == '\0' ? result + "NULL, " : result + "'" + sanitize(to_string(Orbit)) + "', ";
		result = PayloadMass == INT_MIN ? result + "NULL, " : result + to_string(PayloadMass) + ", ";
		result = Supplier[0] == '\0' ? result + "NULL, " : result + "'" + sanitize(to_string(Supplier)) + "', ";
		result = MissionOutcome[0] == '\0' ? result + "NULL, " : result + "'" + sanitize(to_string(MissionOutcome)) + "', ";
		result = DragonSerial == nullptr ? result + "NULL, " : result + "'" + to_string(DragonSerial->SerialNumber, 4) + "', ";
		result += to_string(MissionNumber->MissionNumber) + ", ";
		result += to_string(CrewMembers);
		result += ")";
		return result;
	}
	static string createTable() {
		return "create table " + getName() + " (Title varchar(100) not null,Orbit varchar(100),PayloadMass int,Supplier varchar(255),MissionOutcome varchar(500),DragonSerial char(4),MissionNumber int not null, CrewMembers int, primary key(Title),foreign key(DragonSerial) references Dragon(SerialNumber),foreign key(MissionNumber) references Mission(MissionNumber))";
	}
	static string getName() { return "Payload";  }
};
LinkedList<Payload> Payloads;
Payload* createPayload(string Title, string Orbit, int PayloadMass, string Supplier, string MissionOutcome, Dragon* DragonSerial, Mission* MissionNumber, int CrewMembers) {
	Payload p;
	strcpy(Title, p.Title, 100);
	strcpy(Orbit, p.Orbit, 100);
	p.PayloadMass = PayloadMass;
	strcpy(Supplier, p.Supplier, 255);
	strcpy(MissionOutcome, p.MissionOutcome, 500);
	p.DragonSerial = DragonSerial;
	p.MissionNumber = MissionNumber;
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
		result += to_string(p->Title) + V() + to_string(p->Orbit) + V() + payloadMass + V() + to_string(p->Supplier) + V() + to_string(p->MissionOutcome) + V()
			+ dragonSerial + V() + to_string(p->MissionNumber->MissionNumber) + V() + to_string(p->CrewMembers) + T();
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

			cout << "Creating tables..." << endl;
			//Re-create the dropped tables
			executeSQL(Booster::createTable());
			executeSQL(Dragon::createTable());
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