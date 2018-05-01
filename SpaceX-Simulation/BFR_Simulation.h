#pragma once
#include "Utility.h"

int BFS_SUBORBITAL_MAX_FLIGHTS_PER_DAY = 3;
int BFS_COLONY_FLIGHTS_PER_MONTH = 15;
int BFR_MAX_REFLIGHTS = 100;
int BFR_SUCCESSFUL_LAUNCHES_PER_1000 = 9999;
int BFR_SUCCESSFUL_LANDINGS_PER_1000 = 9998;
int BFS_MINIMUM_PAYLOAD_MASS = 50000;
int BFS_MAXIMUM_PAYLOAD_MASS = 150000;
int BFS_COLONY_STAY_DURATION = 10;
int BFS_CREW_CAPACITY = 100;
int BFS_SUBORBITAL_FLIGHT_CHANCE = 900;
bool COUNT_SUBORBITAL = true;

Date BFR_FLIGHT_OPERATIONAL = Date("2023-12-26");


struct BFRPlatform {
	LaunchSite* launchSite;
	LandingSite* landingSite;

	string orbit;

	Hanger<Dragon> BFSs;
	Hanger<Booster> BFRs;

	BFRPlatform(string SiteID, string Name, string Location, string orbitStr="Suborbital") {
		launchSite = createLaunchSite(SiteID + "_lch", Name + " Launch Platform", Location, 1, 1);
		landingSite = createLandingSite(SiteID + "_lnd", Name + " Landing Platform", 1);
		orbit = orbitStr;
	}

	Booster* getBooster() {
		if (BFRs.getNumOfVehicles() > 0) return BFRs.getFirstVehicle();
		return createBooster(Booster::getNextAvailableBoosterID(), "Active", "BFR", 6);
	}
	Dragon* getDragon() {
		if (BFSs.getNumOfVehicles() > 0) return BFSs.getFirstVehicle();
		return createDragon(Dragon::getNextAvailableSerialNumber(), "Freshly made BFS", 1, 1);
	}
};

vector<BFRPlatform> planetaryPads;
void addPlanetaryBFRPads() {
	planetaryPads.emplace_back("ccg", "Chicago", "Earth, North America, Illinois");
	planetaryPads.emplace_back("nyc", "New York City", "Earth, North America, New York");
	planetaryPads.emplace_back("lag", "Los Angeles", "Earth, North America, California");
	planetaryPads.emplace_back("ldn", "London", "Earth, Europe, United Kingdom");
	planetaryPads.emplace_back("bjg", "Beijing", "Earth, Asia, China");
	planetaryPads.emplace_back("sol", "Seoul", "Earth, Asia, South Korea");
	planetaryPads.emplace_back("tko", "Tokyo", "Earth, Asia, Japan");
	planetaryPads.emplace_back("sdy", "Sydney", "Earth, Australia, New South Wales");
	planetaryPads.emplace_back("hnk", "Hong Kong", "Earth, Asia, China");
	planetaryPads.emplace_back("sgp", "Singapore", "Earth, Asia, Singapore");
	planetaryPads.emplace_back("trt", "Toronto", "Earth, North America, Canada");
	planetaryPads.emplace_back("bgk", "Bangkok", "Earth, Asia, Thailand");
	planetaryPads.emplace_back("dbi", "Dubai", "Earth, Middle East, United Arab Emirates");
	planetaryPads.emplace_back("par", "Paris", "Earth, Europe, France");
	planetaryPads.emplace_back("hlu", "Honolulu", "Earth, Pacific, Hawaii");
	planetaryPads.emplace_back("dli", "Delhi", "Earth, South Asia, India");
	planetaryPads.emplace_back("mbn", "Melbourne", "Earth, Australia, Victoria");
	planetaryPads.emplace_back("cpt", "Cape Town", "Earth, Africa, South Africa");
	planetaryPads.emplace_back("bua", "Buenos Aires", "Earth, South America, Argentina");
	planetaryPads.emplace_back("jhb", "Johannesburg", "Earth, Africa, South Africa");
	planetaryPads.emplace_back("ada", "Adelaide", "Earth, Australia, South Australia");
	planetaryPads.emplace_back("ack", "Auckland", "Earth, Southwestern Pacific, New Zealand");
	planetaryPads.emplace_back("atn", "Athens", "Earth, Southeastern Europe, Greece");
}

vector<BFRPlatform> extraPlanetaryPads;
void addExtraPlanetaryBFRPads() {
	extraPlanetaryPads.emplace_back("mba", "Moon Base Alpha", "Moon, East Mare Imbrium, Archimedes Crater", "Moon");
	extraPlanetaryPads.emplace_back("mrs", "Elon City", "Mars, Equatorial Highlands, Elysium Mons", "Mars");
}

Payload* createBFSPayload(string destination, Mission* missionNumber, LandingSite* landingSite, Dragon* BFS, string missionSuccess, default_random_engine &e) {
	uniform_int_distribution<int> payloadMassSelector(BFS_MINIMUM_PAYLOAD_MASS, BFS_MAXIMUM_PAYLOAD_MASS);
	int payloadMass = payloadMassSelector(e);
	uniform_int_distribution<int> crewMemberSelector(1, BFS_CREW_CAPACITY);
	int crewCapacity = crewMemberSelector(e);
	return destination == "Moon" || destination == "Mars" ? createPayload(destination + " Colony Mission", missionNumber, landingSite, BFS, destination, payloadMass, "SpaceX", missionSuccess, crewCapacity) : createPayload("Suborbital Passenger Service", missionNumber, landingSite, BFS, destination, payloadMass, "SpaceX", missionSuccess, crewCapacity);
}

void planetaryBFRFlight(default_random_engine &e) {
	uniform_int_distribution<int> locations(0, planetaryPads.size() - 1);
	int from = locations(e);
	int to = locations(e);
	while (from == to) to = locations(e);
	bool launchSuccess = chanceOutOf1000(BFR_SUCCESSFUL_LAUNCHES_PER_1000, e);
	bool landingSuccess = chanceOutOf1000(BFR_SUCCESSFUL_LANDINGS_PER_1000, e);
	string missionSuccess = launchSuccess ? "Complete success." : "Catastrophic failure.";
	Mission* mission = createMission(Mission::getNextAvailableMissionNumber(), planetaryPads[from].launchSite, to_string(planetaryPads[from].launchSite->Name) + " to " + to_string(planetaryPads[to].landingSite->Name), "Suborbital BFR passenger service.", today, launchSuccess);
	Dragon* BFS = planetaryPads[from].getDragon();
	Booster* BFR = planetaryPads[from].getBooster();
	Payload* payload = createBFSPayload("Suborbital", mission, planetaryPads[to].landingSite, BFS, missionSuccess, e);
	string landingString = launchSuccess ? landingSuccess ? "Recovered nominally." : "Core destroyed" : "Precluded by failed launch";
	flownBy* flight = createFlownBy(BFR, mission, planetaryPads[from].landingSite, landingString, landingSuccess);
	BFR->flights++;

	if (launchSuccess) {
		if (landingSuccess) {
			if (BFR->flights < BFR_MAX_REFLIGHTS) {
				planetaryPads[from].BFRs.addVehicle(BFR);
			}
			else {
				strcpy("Retired", BFR->FlightStatus, 50);
			}
		}
		else {
			strcpy("Destroyed", BFR->FlightStatus, 50);
		}
		strcpy("Last flown on a suborbital flight on " + to_string(today), BFS->Description, 400);
		planetaryPads[to].BFSs.addVehicle(BFS);
	}
	else {
		strcpy("Destroyed", BFR->FlightStatus, 50);
		strcpy("Destroyed on failed launch on " + to_string(today), BFS->Description, 400);
	}
}

void extraPlanetaryBFRFlight(default_random_engine &e) {
	uniform_int_distribution<int> locations(0, planetaryPads.size() - 1);
	uniform_int_distribution<int> destinations(0, extraPlanetaryPads.size() - 1);
	int from = locations(e);
	int to = destinations(e);

	bool launchSuccess = chanceOutOf1000(BFR_SUCCESSFUL_LAUNCHES_PER_1000, e);
	bool landingSuccess = chanceOutOf1000(BFR_SUCCESSFUL_LANDINGS_PER_1000, e);
	string missionSuccess = launchSuccess ? "Complete success." : "Catastrophic failure.";
	Mission* mission = createMission(Mission::getNextAvailableMissionNumber(), planetaryPads[from].launchSite, "Colony Mission to "+to_string(extraPlanetaryPads[to].landingSite->Name), "Colony BFR mission.", today, launchSuccess);
	Dragon* BFS = planetaryPads[from].getDragon();
	Booster* BFR = planetaryPads[from].getBooster();
	Payload* payload = createBFSPayload(extraPlanetaryPads[to].orbit, mission, extraPlanetaryPads[to].landingSite, BFS, missionSuccess, e);
	string landingString = launchSuccess ? landingSuccess ? "Recovered nominally." : "Core destroyed" : "Precluded by failed launch";
	flownBy* flight = createFlownBy(BFR, mission, planetaryPads[from].landingSite, landingString, landingSuccess);
	BFR->flights++;

	if (launchSuccess) {
		if (landingSuccess) {
			if (BFR->flights < BFR_MAX_REFLIGHTS) {
				planetaryPads[from].BFRs.addVehicle(BFR);
			}
			else {
				strcpy("Retired", BFR->FlightStatus, 50);
			}
		}
		else {
			strcpy("Destroyed", BFR->FlightStatus, 50);
		}
		strcpy("Last flown on a colony mission on " + to_string(today), BFS->Description, 400);
		extraPlanetaryPads[to].BFSs.addVehicle(BFS);
	}
	else {
		strcpy("Destroyed", BFR->FlightStatus, 50);
		strcpy("Destroyed on failed launch on " + to_string(today), BFS->Description, 400);
	}
}

void BFRFlights(default_random_engine &e) {
	if (chanceOutOf30(BFS_COLONY_FLIGHTS_PER_MONTH, e)) { //We are sending out colonists today!
		cout << "Generating BFR colony mission. Flight #" << Mission::getNextAvailableMissionNumber() << "..." << endl;
		extraPlanetaryBFRFlight(e);
		currentNumberOfFlights++;
	}
	if (chanceOutOf1000(BFS_SUBORBITAL_FLIGHT_CHANCE, e)) {
		uniform_int_distribution<int> howManyFlights(0, BFS_SUBORBITAL_MAX_FLIGHTS_PER_DAY);
		int flights = howManyFlights(e);
		for (int i = 0; i < flights; i++) { //Perform airplane like operations
			cout << "Generating BFR Earth-to-Earth mission. Flight #" << Mission::getNextAvailableMissionNumber() << "..." << endl;
			planetaryBFRFlight(e);
			if(COUNT_SUBORBITAL)
				currentNumberOfFlights++;
		}
	}

	//Create return flights from colonies
	for (int i = 0; i < extraPlanetaryPads.size(); i++) {
		vector<Dragon*> temp = extraPlanetaryPads[i].BFSs.getVehiclesOlderThan(today, BFS_COLONY_STAY_DURATION);
		while (temp.size() > 0) {
			Dragon* BFS = temp.back();
			temp.pop_back();
			Mission* mission = createMission(Mission::getNextAvailableMissionNumber(), extraPlanetaryPads[i].launchSite, "BFS return to Earth flight", "BFS is returning to earth from " + to_string(extraPlanetaryPads[i].launchSite->Name), today, 1);
			currentNumberOfFlights++;
			uniform_int_distribution<int> locations(0, planetaryPads.size() - 1);
			int to = locations(e);
			strcpy("Returned to Earth from completed colony mission on " + to_string(today), BFS->Description, 400);
			Payload* payload = createPayload("BFS return flight", mission, planetaryPads[to].landingSite, BFS, "Earth", BFS_MINIMUM_PAYLOAD_MASS, "SpaceX", "Success", 0);
			planetaryPads[to].BFSs.addVehicle(BFS);
		}
	}
}