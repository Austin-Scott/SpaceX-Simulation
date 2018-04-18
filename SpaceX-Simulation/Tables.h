#pragma once
#include <string>
#include <vector>
#include <sstream>

const char ValueDeliminator = '|';
const char TupleDeliminator = '\n';
const string NullIndicator = "\\N";

#include "Utility.h"

/*
Each table must have the following public elements:
 string getTuple();                  <-Returns a SQL formatted string of the current object
 string getRow();                    <-Returns a TXT dataloading formattted string of the current object
 static LinkedList<table_type> LL;   <-LinkedList of the current table type to store all rows of the table (instances of the struct)
 static string getStructure();       <-Returns SQL formatted string of the attributes of the table when in a database
 static string getName();            <-Returns the name of the table
*/

//Booster table
struct Booster {
	char BoosterID[5]; //Primary Key
	char FlightStatus[50];
	char CoreType[25];
	int BlockNumber;

	//Stored data not used in final database
	int flights;

	string getTuple() {
		string result = "( '" + to_string(BoosterID, 5) + "', ";
		result = FlightStatus[0] == '\0' ? result + "NULL, " : result + "'" + sanitize(to_string(FlightStatus))+ "', ";
		result = CoreType[0] == '\0' ? result + "NULL, " : result + "'" + sanitize(to_string(CoreType)) + "', ";
		result = BlockNumber == INT_MIN ? result + "NULL" : result + to_string(BlockNumber);
		result += ")";
		return result;
	}

	string getRow() {
		string blockNumber = BlockNumber != INT_MIN ? to_string(BlockNumber) : NullIndicator;
		return to_string(BoosterID, 5) + V() + to_string(FlightStatus) + V() + to_string(CoreType) + V() + blockNumber + T();
	}

	static LinkedList<Booster> LL;

	static string getStructure() {
		return " ( BoosterID char(5) not null, FlightStatus varchar(50), CoreType varchar(25), BlockNumber int, primary key (BoosterID))";
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
LinkedList<Booster> Booster::LL = LinkedList<Booster>();
int Booster::highestBoosterID = 0;
Booster* createBooster(string BoosterID, string FlightStatus, string CoreType, int BlockNumber) {
	Booster b;
	strcpy(BoosterID, b.BoosterID, 5, false);
	strcpy(FlightStatus, b.FlightStatus, 50);
	strcpy(CoreType, b.CoreType, 25);
	b.BlockNumber = BlockNumber;
	b.flights = 0;

	int boosterNum = atoi(BoosterID.substr(1, 4).c_str());
	if (boosterNum > Booster::highestBoosterID) Booster::highestBoosterID = boosterNum;

	Booster::LL.push_back(b);
	return Booster::LL.back();
}
Booster* findBooster(string BoosterID) {
	for (auto i = Booster::LL.begin(); i.hasNext(); i.operator++()) {
		Booster* p = &i;
		if (stringcmp(BoosterID, p->BoosterID, 5, false))
			return p;
	}
	return nullptr;
}
//End Booster table

//Dragon table
struct Dragon {
	char SerialNumber[4]; //Primary Key
	char Description[400];
	int BFS; //-1=null, 0=false, 1=true
	int FlightActive; //-1=null, 0=false, 1=true

	string getTuple() {
		string result = "( '" + to_string(SerialNumber, 4) + "', ";
		result = Description[0] == '\0' ? result + "NULL, " : result + "'" + sanitize(to_string(Description)) + "', ";
		result += boolToString(BFS) + ", ";
		result += boolToString(FlightActive);
		result += ")";
		return result;
	}

	string getRow() {
		return to_string(SerialNumber, 4) + V() + to_string(Description) + V() + boolToListString(BFS) + V() + boolToListString(FlightActive) + T();
	}

	static LinkedList<Dragon> LL;

	static string getStructure() {
		return " ( SerialNumber char(4) not null, Description varchar(400), BFS boolean, FlightActive boolean, primary key(SerialNumber) )";
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
LinkedList<Dragon> Dragon::LL = LinkedList<Dragon>();
int Dragon::highestSerialNumber = 0;
Dragon* createDragon(string SerialNumber, string Description, int BFS, int FlightActive) {
	Dragon d;
	strcpy(SerialNumber, d.SerialNumber, 4, false);
	strcpy(Description, d.Description, 400);
	d.FlightActive = FlightActive;
	d.BFS = BFS;

	int capsuleNum = atoi(SerialNumber.substr(1, 3).c_str());
	if (capsuleNum > Dragon::highestSerialNumber) Dragon::highestSerialNumber = capsuleNum;

	Dragon::LL.push_back(d);
	return Dragon::LL.back();
}
Dragon* findDragon(string SerialNumber) {
	for (auto i = Dragon::LL.begin(); i.hasNext(); i.operator++()) {
		Dragon* p = &i;
		if (stringcmp(SerialNumber, p->SerialNumber, 4, false))
			return p;
	}
	return nullptr;
}
//End Dragon table

//LaunchSite table
struct LaunchSite {
	char SiteID[25]; //Primary Key
	char Name[255];
	char Location[255];
	int Active; //-1=null, 0=false, 1=true
	int BFR; //-1=null, 0=false, 1=true

	string getTuple() {
		string result = "( '" + to_string(SiteID) + "', ";
		result = Name[0] == '\0' ? result + "NULL, " : result + "'" + sanitize(to_string(Name)) + "', ";
		result = Location[0] == '\0' ? result + "NULL, " : result + "'" + sanitize(to_string(Location)) + "', ";
		result += boolToString(Active) + ", ";
		result += boolToString(BFR);
		result += ")";
		return result;
	}

	string getRow() {
		return to_string(SiteID) + V() + to_string(Name) + V() + to_string(Location) + V() + boolToListString(Active) + V() + boolToListString(BFR) + T();
	}

	static LinkedList<LaunchSite> LL;

	static string getStructure() {
		return " (SiteID varchar(25) not null, Name varchar(255), Location varchar(255), Active boolean, BFR boolean, primary key(SiteID))";
	}
	static string getName() { return "LaunchSite";  }
};
LinkedList<LaunchSite> LaunchSite::LL = LinkedList<LaunchSite>();
LaunchSite* createLaunchSite(string SiteID, string Name, string Location, int Active, int BFR) {
	LaunchSite l;
	strcpy(SiteID, l.SiteID, 25);
	strcpy(Name, l.Name, 255);
	strcpy(Location, l.Location, 255);
	l.Active = Active;
	l.BFR = BFR;
	LaunchSite::LL.push_back(l);
	return LaunchSite::LL.back();
}
LaunchSite* findLaunchSite(string SiteID) {
	for (auto i = LaunchSite::LL.begin(); i.hasNext(); i.operator++()) {
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
//End LaunchSite table

//LandingSite table
struct LandingSite {
	char SiteID[25]; //Primary Key
	char Name[255];
	int BFR; //-1=null, 0=false, 1=true

	string getTuple() {
		string result = "( '" + to_string(SiteID) + "', ";
		result = Name[0] == '\0' ? result + "NULL, " : result + "'" + sanitize(to_string(Name)) + "', ";
		result += boolToString(BFR);
		result += ")";
		return result;
	}

	string getRow() {
		return to_string(SiteID) + V() + to_string(Name) + V() + boolToListString(BFR) + T();
	}

	static LinkedList<LandingSite> LL;

	static string getStructure() {
		return "(SiteID varchar(25) not null,Name varchar(255), BFR boolean, primary key(SiteID))";
	}
	static string getName() { return "LandingSite"; }
};
LinkedList<LandingSite> LandingSite::LL = LinkedList<LandingSite>();
LandingSite* createLandingSite(string SiteID, string Name, int BFR) {
	LandingSite l;
	strcpy(SiteID, l.SiteID, 25);
	strcpy(Name, l.Name, 255);
	l.BFR = BFR;
	LandingSite::LL.push_back(l);
	return LandingSite::LL.back();
}
LandingSite* findLandingSite(string SiteID) {
	for (auto i = LandingSite::LL.begin(); i.hasNext(); i.operator++()) {
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
//End LandingSite table

//Mission table
struct Mission {
	int MissionNumber; //Primary Key
	LaunchSite* LaunchSiteName;
	char Title[255];
	char Description[400];
	Date date;
	int LaunchSuccess; //-1=null, 0=false, 1=true

	string getTuple() {
		string result = "( " + to_string(MissionNumber) + ", ";
		result = LaunchSiteName == nullptr ? result + "NULL, " : result + "'" + to_string(LaunchSiteName->SiteID) + "', ";
		result = Title[0] == '\0' ? result + "NULL, " : result + "'" + sanitize(to_string(Title)) + "', ";
		result = Description[0] == '\0' ? result + "NULL, " : result + "'" + sanitize(to_string(Description)) + "', ";
		result += "'" + to_string(date) + "', ";
		result += boolToString(LaunchSuccess);
		result += ")";
		return result;
	}
	string getRow() {
		return to_string(MissionNumber) + V() + to_string(LaunchSiteName->Name) + V() + to_string(Title) + V() + to_string(Description) + V() + to_string(date) + V() + boolToListString(LaunchSuccess) + T();
	}

	static LinkedList<Mission> LL;

	static string getStructure() {
		return " (MissionNumber int not null, LaunchSite varchar(25) not null, Title varchar(255), Description varchar(400), Date date, LaunchSuccess boolean, primary key(MissionNumber),foreign key(LaunchSite) references LaunchSite(SiteID))";
	}
	static string getName() { return "Mission";  }

	friend Mission* createMission(int MissionNumber, LaunchSite* LaunchSiteName, string Title, string Description, Date date, int LaunchSuccess);
	static int getNextAvailableMissionNumber() {
		return highestMissionNumber + 1;
	}

private:
	static int highestMissionNumber;
};
LinkedList<Mission> Mission::LL = LinkedList<Mission>();
int Mission::highestMissionNumber = 0;
Mission* createMission(int MissionNumber, LaunchSite* LaunchSiteName, string Title, string Description, Date date, int LaunchSuccess) {
	Mission m;
	m.MissionNumber = MissionNumber;
	m.LaunchSiteName = LaunchSiteName;
	strcpy(Title, m.Title, 255);
	strcpy(Description, m.Description, 400);
	m.date = date;
	m.LaunchSuccess = LaunchSuccess;

	if (MissionNumber > Mission::highestMissionNumber) Mission::highestMissionNumber = MissionNumber;

	Mission::LL.push_back(m);
	return Mission::LL.back();
}
Mission* findMission(int MissionNumber) {
	for (auto i = Mission::LL.begin(); i.hasNext(); i.operator++()) {
		Mission* p = &i;
		if (p->MissionNumber == MissionNumber) return p;
	}
	return nullptr;
}
//End Mission Table

//flownBy table
struct flownBy {
	Booster* BoosterID; //Primary Key
	Mission* MissionNumber; //Primary Key
	LandingSite* LandSite; //Foreign Key
	char LandingOutcome[50];
	int LandingSuccess; //-1=null, 0=false, 1=true

	string getTuple() {
		string result = "( '" + to_string(BoosterID->BoosterID, 5) + "', " + to_string(MissionNumber->MissionNumber) + ", ";
		result = LandSite == nullptr ? result + "NULL, " : result + "'" + sanitize(to_string(LandSite->SiteID)) + "', ";
		result = LandingOutcome[0] == '\0' ? result + "NULL, " : result + "'" + sanitize(to_string(LandingOutcome)) + "', ";
		result += boolToString(LandingSuccess);
		result += ")";
		return result;
	}
	string getRow() {
		string landSite = LandSite == nullptr ? NullIndicator : to_string(LandSite->SiteID);
		return to_string(BoosterID->BoosterID, 5) + V() + to_string(MissionNumber->MissionNumber) + V() + landSite + V() + to_string(LandingOutcome) + V() + boolToListString(LandingSuccess) + T();
	}

	static LinkedList<flownBy> LL;

	static string getStructure() {
		return "(BoosterID char(5) not null,MissionNumber int not null,LandingSite varchar(25),LandingOutcome varchar(50), LandingSuccess boolean, primary key(BoosterID, MissionNumber),foreign key(BoosterID) references Booster(BoosterID),foreign key(MissionNumber) references Mission(MissionNumber), foreign key(LandingSite) references LandingSite(SiteID))";
	}
	static string getName() { return "flownBy";  }
};
LinkedList<flownBy> flownBy::LL = LinkedList<flownBy>();
flownBy* createFlownBy(Booster* BoosterID, Mission* MissionNumber, LandingSite* LandingSite, string LandingOutcome, int LandingSuccess) {
	flownBy f;
	f.BoosterID = BoosterID;
	f.MissionNumber = MissionNumber;
	f.LandSite = LandingSite;
	strcpy(LandingOutcome, f.LandingOutcome, 50);
	f.LandingSuccess = LandingSuccess;
	flownBy::LL.push_back(f);
	return flownBy::LL.back();
}
flownBy* findFlownBy(Booster* BoosterID, Mission* MissionNumber) {
	for (auto i = flownBy::LL.begin(); i.hasNext(); i.operator++()) {
		flownBy* p = &i;
		if (p->BoosterID == BoosterID&&p->MissionNumber == MissionNumber)
			return p;
	}
	return nullptr;
}
//End flownBy table

//Payload table
struct Payload {
	char Title[100]; //Primary Key
	Mission* MissionNumber; //Primary Key
	LandingSite* DestinationSite; //Foreign Key
	Dragon* DragonSerial; //Foreign Key
	char Orbit[50];
	int PayloadMass;
	char Supplier[50];
	char MissionOutcome[50];
	int CrewMembers;

	string getTuple() {
		string result = "( '" + sanitize(to_string(Title)) + "', ";
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
	string getRow() {
		string payloadMass = PayloadMass != INT_MIN ? to_string(PayloadMass) : NullIndicator;
		string dragonSerial = DragonSerial != nullptr ? to_string(DragonSerial->SerialNumber, 4) : NullIndicator;
		string destinationSite = DestinationSite == nullptr ? NullIndicator : to_string(DestinationSite->SiteID);
		return to_string(Title) + V() + to_string(MissionNumber->MissionNumber) + V() + destinationSite + V() + dragonSerial + V() + to_string(Orbit) + V() + payloadMass + V() + to_string(Supplier) + V() + to_string(MissionOutcome) + V() + to_string(CrewMembers) + T();
	}

	static LinkedList<Payload> LL;

	static string getStructure() {
		return "(Title varchar(100) not null, MissionNumber int not null, DestinationSite varchar(25), DragonSerial char(4), Orbit varchar(50),PayloadMass int,Supplier varchar(50),MissionOutcome varchar(50), CrewMembers int, primary key(Title, MissionNumber), foreign key(DestinationSite) references LandingSite(SiteID), foreign key(DragonSerial) references Dragon(SerialNumber),foreign key(MissionNumber) references Mission(MissionNumber))";
	}
	static string getName() { return "Payload";  }
};
LinkedList<Payload> Payload::LL = LinkedList<Payload>();
Payload* createPayload(string Title, Mission* MissionNumber, LandingSite* DestinationSite, Dragon* DragonSerial, string Orbit, int PayloadMass, string Supplier, string MissionOutcome, int CrewMembers) {
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
	Payload::LL.push_back(p);
	return Payload::LL.back();
}
Payload* findPayload(string Title) {
	for (auto i = Payload::LL.begin(); i.hasNext(); i.operator++()) {
		Payload* p = &i;
		if (stringcmp(Title, p->Title, 100))
			return p;
	}
	return nullptr;
}
//End Payload table

//Functions involving all tables

template<class T> void deleteData() {
	T::LL.deleteAll();
}

void cleanAllData() {
	deleteData<Booster>();
	deleteData<flownBy>();
	deleteData<Mission>();
	deleteData<Payload>();
	deleteData<Dragon>();
	deleteData<LaunchSite>();
	deleteData<LandingSite>();
}

template<class T> string getRows() {
	string result = "";
	for (auto i = T::LL.begin(); i.hasNext(); i.operator++()) {
		result += (&i)->getRow();
	}
	return result;
}

template<class T> void writeToFile() {
	ofstream file("output/" + T::getName() + ".txt", ios::out);
	if (file) {
		file << getRows<T>();
	}
	file.close();
}

void writeResultsToFiles() {
	cout << "Outputing results to files..." << endl;

	writeToFile<Booster>();
	writeToFile<flownBy>();
	writeToFile<Mission>();
	writeToFile<Payload>();
	writeToFile<Dragon>();
	writeToFile<LaunchSite>();
	writeToFile<LandingSite>();

	cout << "Done." << endl;
}

//MySQL database functions

template<class T> void dropTable() {
	executeSQL("DROP TABLE IF EXISTS " + T::getName());
}

template<class T> void dropTuples() {
	executeSQL("DELETE FROM " + T::getName());
}

template<class T> void insertTuples(string tuples) {
	executeSQL("INSERT INTO " + T::getName() + " VALUES " + tuples);
}

template<class T> void createTable() {
	executeSQL("CREATE TABLE " + T::getName() + " " + T::getStructure());
}

template<class T> void addTuples() {
	string tuples = "";
	cout << "Inserting tuples into " << T::getName() << "..." << endl;
	for (auto iter = T::LL.begin(); iter.hasNext(); iter.operator++()) {
		tuples += (&iter)->getTuple();
		if ((iter + 1).hasNext()) tuples += ", ";
	}
	insertTuples<T>(tuples);
}

void updateDatabase(string address, string schema, string username, string password, bool soft) {
	cout << "Connecting to database at " << address << "..." << endl;
	connectToDatabase(address, schema, username, password);

	if (isSQLConnected()) {

		if (soft) {
			cout << "Dropping all tuples from tables..." << endl;
			//drop all tuples
			dropTuples<flownBy>();
			dropTuples<Payload>();
			dropTuples<Booster>();
			dropTuples<Dragon>();
			dropTuples<Mission>();
			dropTuples<LaunchSite>();
			dropTuples<LandingSite>();
		}
		else {

			cout << "Dropping any pre-existing tables..." << endl;
			//Drop tables if they exist
			dropTable<flownBy>();
			dropTable<Payload>();
			dropTable<Booster>();
			dropTable<Dragon>();
			dropTable<Mission>();
			dropTable<LaunchSite>();
			dropTable<LandingSite>();

			cout << "Creating tables..." << endl;
			//Re-create the dropped tables
			createTable<Booster>();
			createTable<Dragon>();
			createTable<LandingSite>();
			createTable<LaunchSite>();
			createTable<Mission>();
			createTable<flownBy>();
			createTable<Payload>();

		}

		//Populate the database
		addTuples<Booster>();
		addTuples<Dragon>();
		addTuples<LaunchSite>();
		addTuples<LandingSite>();
		addTuples<Mission>();
		addTuples<flownBy>();
		addTuples<Payload>();

		cout << "Complete. Disconnecting from database." << endl;

		disconnectFromDatabase();

	}
	else {
		cout << "Could not connect to database. Moving on." << endl;
	}
}