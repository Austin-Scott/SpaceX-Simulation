#pragma once
#include <random>
#include <time.h>
#include <algorithm>

int FLIGHTS_TO_PERFORM = 2000;
#define AVERAGE_FLIGHTS_PER_MONTH 5
#define AVERAGE_FALCON_HEAVY_FLIGHTS_PER_1000 75
#define AVERAGE_SUCCESSFUL_LAUNCHES_PER_1000 996
#define AVERAGE_SUCCESSFUL_LANDINGS_PER_1000 983
#define BLOCK_V_MAX_REFLIGHTS 12
#define LESSER_BLOCK_MAX_REFLIGHTS 2
#define WEIGHT_OF_DRAGON 4200
#define MINIMUM_PAYLOAD_WEIGHT 2000
#define DRAGON_DAYS_BETWEEN_FLIGHTS 30
#define MINIMUM_FLIGHT_READY_CORES 3
#define MINIMUM_FLIGHT_READY_DRAGONS 2
#define MAXIMUM_SECONDARY_PAYLOADS 3
#define AVERAGE_FLIGHTS_WITH_SECONDARY_PAYLOADS_PER_1000 250
#define DRAGON_CREW_CAPACITY 7


int currentNumberOfFlights = 0;
Date today(endDate);

const int numberOfLandingSites = 8;
string landingSites[] = { "OCISLY", "JRTI", "ASOG", "LZ-1", "LZ-2", "VAFB LZ-1", "VAFB LZ-2", "ST LZ-1" };

struct LaunchSiteName {
	string name;
	string description;
	bool landingSitesAvailable[numberOfLandingSites];
};
const int numOfLaunchSites = 4;
LaunchSiteName launchSites[] = { {"ccafs_slc_40", "Cape Canaveral Air Force Station Space Launch Complex 40", {true, false, true, true, true, false, false, false}}, {"vafb_slc_4e", "Vandenberg Air Force Base Space Launch Complex 4E", {false, true, false, false, false, true, true, false}}, {"ksc_lc_39a", "Kennedy Space Center Historic Launch Complex 39A", {true, false, true, true, true, false, false, false} }, {"stls", "SpaceX South Texas Launch Site", {true, false, true, false, false, false, false, true}} };

struct Destination {
	string name;
	int max9Payload;
	int maxHPayload;
	int requiredLaunchSite; //-1 means choose any launchSite. Otherwise index of required launchsite.
	int excludedLaunchSite; //Only meaningful if requiredLaunchSite is set to -1. Excludes a launchSite. -1 means don't exclude anywhere. Otherwise index of excluded launchsite.
};
const int numOfDestinations = 8;
Destination destinations[] = { {"LEO",22800,63800, -1, 1}, {"PO", 22800,63800, 1, -1}, {"ISS", 22800, 63800, -1, 1}, {"GTO", 8300, 26700, -1, 1}, {"Mars", 4020, 16800, 3, -1}, {"Heliocentric orbit", 6300, 23500, -1, 1 }, {"SSO", 22800,63800, 1, -1}, {"TLI", 6300, 17216, -1, 1} };

struct PayloadName {
	string name;
	string supplier;
	bool requiresDragon;
	int destination; //-1 means choose any destination. Otherwise index of required destination.
	int suffix;
	bool mustBePrimary;
	string getName() {
		string result = name + to_string(suffix);
		suffix++;
		return result;
	}
};
const int numOfPayloadNames=23;
PayloadName payloadNames[] = { {"CRS-", "NASA (Cargo Resupply Services)", true, 2, 15, true}, {"CCS-", "NASA (Commercial Crew Services)", true, 2, 1, true}, {"Red Dragon-", "SpaceX", true, 4, 1, true}, {"SES-", "SES", false, 3, 12, false}, {"NROL-", "NROL", false, -1, 77, false}, {"Iridium NEXT ", "Iridium Communications", false, 1, 6, false}, {"STARLINK-", "SpaceX", false, 0, 1, true}, {"GovSat-", "GovSat", false, 3, 2, false}, {"X37B OTV-", "USAF", false, -1, 6, true}, {"Inmarsat-", "Inmarsat", false, 3, 6, false}, {"Space Tourism-", "SpaceX", true, -1, 1, true}, {"FormoSat-", "NSPO (Taiwan)", false, 6, 6, false }, {"EchoStar ", "EchoStar", false, 3, 106, false}, {"BulgariaSat-", "Bulgaria Sat", false, 3, 2, false}, {"Amos-", "Spacecom", false, 3, 7, false}, {"JCSAT-", "SKY Perfect JCSAT Group", false, 3, 17, false}, {"KoreaSat ", "KT Corporation", false, 3, 6, false}, {"Thaicom ", "Thaicom", false, 3, 9, false}, {"AsiaSat ", "AsiaSat", false, 0, 9, false}, {"Jason-", "NASA", false, 1, 4, false}, {"Eutelsat ", "Eutelsat", false, 3, 118, false}, {"Microsat-", "SpaceX", false, -1, 3, false}, {"Cubesats ", "NRO", false, -1, 2, false} };

bool chanceOutOf1000(int n, default_random_engine &e) {
	uniform_int_distribution<int> dist(1, 1000);
	int guess = dist(e);
	return guess <= n;
}

bool chanceOutOf30(int n, default_random_engine &e) {
	uniform_int_distribution<int> dist(1, 30);
	int guess = dist(e);
	return guess <= n;
}

void updateHangers() {
	flightActiveDragons.addVehicles(dragonsInSpace.getVehiclesOlderThan(today, DRAGON_DAYS_BETWEEN_FLIGHTS), today);
	while (flightActiveCores.getNumOfVehicles() < MINIMUM_FLIGHT_READY_CORES) {
		string boosterID = "B";
		boosterID += padWithZeros(to_string(highestBoosterNumber), 4);
		highestBoosterNumber++;
		flightActiveCores.addVehicle(createBooster(boosterID, "Active", 5));
	}
	while (flightActiveDragons.getNumOfVehicles() < MINIMUM_FLIGHT_READY_DRAGONS) {
		string capsuleID = "C";
		capsuleID += padWithZeros(to_string(highestCapsuleNumber), 3);
		highestCapsuleNumber++;
		flightActiveDragons.addVehicle(createDragon(capsuleID, "Freshly made Dragon Capsule", 1));
	}
}

bool generateMission(default_random_engine &e) {
	//Step 1: Pick payload
	uniform_int_distribution<int> payloadChoices(0, numOfPayloadNames - 1);
	int payloadChoice = payloadChoices(e);
	string payloadName = payloadNames[payloadChoice].getName();
	while(findPayload(payloadName)!=nullptr) payloadName = payloadNames[payloadChoice].getName();
	//Step 2: Pick destination orbit
	int destination;
	if (payloadNames[payloadChoice].destination == -1) {
		uniform_int_distribution<int> destinationChoices(0, numOfDestinations - 1);
		destination = destinationChoices(e);
	}
	else {
		destination = payloadNames[payloadChoice].destination;
	}
	//Step 3: Pick launch site
	int launchSite;
	if (destinations[destination].requiredLaunchSite == -1) {
		uniform_int_distribution<int> launchSiteChoices(0, numOfLaunchSites - 1);
		do {
			launchSite = launchSiteChoices(e);
		} while (launchSite == destinations[destination].excludedLaunchSite);
	}
	else {
		launchSite = destinations[destination].requiredLaunchSite;
	}
	//Step 4: Choose launch vehicle configuration
	bool falconHeavy = chanceOutOf1000(AVERAGE_FALCON_HEAVY_FLIGHTS_PER_1000, e);
	//Step 5: Figure out if mission is possible with current configuration based on payload mass
	int dragonMass = payloadNames[payloadChoice].requiresDragon ? WEIGHT_OF_DRAGON : 0;
	if (!falconHeavy) {
		int minimumPayloadMass = destinations[destination].max9Payload - dragonMass;
		if (minimumPayloadMass < MINIMUM_PAYLOAD_WEIGHT) return generateMission(e); //Start over
	}
	//Step 6: Choose mass of primary payload
	int maxMass = falconHeavy ? destinations[destination].maxHPayload : destinations[destination].max9Payload;
	maxMass -= dragonMass;
	uniform_int_distribution<int> massChoices(MINIMUM_PAYLOAD_WEIGHT, maxMass);
	int primaryMass = massChoices(e)+dragonMass;
	//Step 7: Assign Dragon Capsule if needed
	Dragon* dragonCapsule = nullptr;
	if (payloadNames[payloadChoice].requiresDragon) {
		dragonCapsule = flightActiveDragons.getRandomVehicles(1, e)[0];
	}
	int crewMembers = 0;
	if (payloadChoice == 1 || payloadChoice == 10) {
		uniform_int_distribution<int> chooseCrewMembers(1, DRAGON_CREW_CAPACITY);
		crewMembers = chooseCrewMembers(e);
	}
	//Step 8: Assign Core/s
	vector<Booster*> cores;
	if (falconHeavy) {
		cores = flightActiveCores.getRandomVehicles(3, e);
	}
	else {
		cores = flightActiveCores.getRandomVehicles(1, e);
	}
	//Step 9: Get/Create launchsite reference
	LaunchSite* missionLaunchSite = findOrCreateLaunchSite(launchSites[launchSite].name, launchSites[launchSite].description);
	//Step 10: Create new mission reference
	Mission* mission = createMission(highestMissionNumber, payloadName, "", today, missionLaunchSite, -1);
	highestMissionNumber++;
	//Step 11: Create primary payload reference
	vector<Payload*> payloads;
	payloads.push_back(createPayload(payloadName, destinations[destination].name, primaryMass, payloadNames[payloadChoice].supplier, "", dragonCapsule, mission, crewMembers));
	//Step 12: Add secondary payload references
	if (chanceOutOf1000(AVERAGE_FLIGHTS_WITH_SECONDARY_PAYLOADS_PER_1000, e)) {
		maxMass += dragonMass;
		int secondaryPayloads = 0;
		while (maxMass - primaryMass > MINIMUM_PAYLOAD_WEIGHT && secondaryPayloads<MAXIMUM_SECONDARY_PAYLOADS) {
			//Step 12.1: Choose new payload that does not require dragon and can be sent to the same destination
			int secondaryPayloadChoice;
			do {
				secondaryPayloadChoice = payloadChoices(e);
			} while (payloadNames[secondaryPayloadChoice].requiresDragon || payloadNames[secondaryPayloadChoice].mustBePrimary || (payloadNames[secondaryPayloadChoice].destination != -1 && payloadNames[secondaryPayloadChoice].destination != payloadNames[payloadChoice].destination));
			//Step 12.2: Choose secondary payload's mass
			uniform_int_distribution<int> secondaryPayloadMassChoices(MINIMUM_PAYLOAD_WEIGHT, maxMass - primaryMass);
			int secondaryPayloadMass = secondaryPayloadMassChoices(e);
			//Step 12.3: Create secondary payload reference
			string secondaryPayloadName = payloadNames[secondaryPayloadChoice].getName();
			while(findPayload(secondaryPayloadName)!=nullptr) secondaryPayloadName = payloadNames[secondaryPayloadChoice].getName();
			payloads.push_back(createPayload(secondaryPayloadName, destinations[destination].name, secondaryPayloadMass, payloadNames[secondaryPayloadChoice].supplier, "", dragonCapsule, mission, 0));
			//Step 12.4: Reduce total payload capacity
			primaryMass += secondaryPayloadMass;

			secondaryPayloads++;
		}
	}
	//Step 13: Select landing sites
	vector<string> sites;
	for (int i = 0; i < numberOfLandingSites; i++) {
		if (launchSites[launchSite].landingSitesAvailable[i]) {
			sites.push_back(landingSites[i]);
		}
	}
	random_shuffle(sites.begin(), sites.end());
	//Step 14: Launch!!!
	bool successfulLaunch = chanceOutOf1000(AVERAGE_SUCCESSFUL_LAUNCHES_PER_1000, e);
	mission->LaunchSuccess = successfulLaunch ? 1 : 0;
	//Step 15: Land!!!
	for (auto i : cores) {
		i->flights++;
		string landingOutcome;
		int landingSuccess = -1;
		if (successfulLaunch) {
			if (chanceOutOf1000(AVERAGE_SUCCESSFUL_LANDINGS_PER_1000, e)) {
				landingSuccess = 1;
				landingOutcome = "Success. Core recovered nominally.";
				if ((i->BlockNumber == 5 && i->flights < BLOCK_V_MAX_REFLIGHTS) || (i->BlockNumber != 5 && i->flights < LESSER_BLOCK_MAX_REFLIGHTS)) {
					flightActiveCores.addVehicle(i);
				}
				else {
					strcpy("Retired", i->FlightStatus, 255);
				}
			}
			else {
				landingSuccess = 0;
				landingOutcome = "Rapid unscheduled dissasembly.";
				strcpy("Destroyed", i->FlightStatus, 255);
			}
		}
		else {
			landingOutcome = "Precluded. Core destroyed during failed launch.";
			strcpy("Destroyed", i->FlightStatus, 255);
		}
		flownBy* flight = createFlownBy(i, mission, sites.back(), landingOutcome, landingSuccess);
		sites.pop_back();
	}
	//Step 16: Update payloads
	for (auto i : payloads) {
		if (successfulLaunch) {
			strcpy("Complete success.", i->MissionOutcome, 255);
		}
		else {
			strcpy("Mission failed. Payload destroyed.", i->MissionOutcome, 255);
		}
	}
	if (payloadNames[payloadChoice].requiresDragon && successfulLaunch) {
		if (payloadChoice == 2) {
			string description = "Sent to Mars on " + to_string(today) + " with no hope to return.";
			dragonCapsule->FlightActive = 0;
			strcpy(description, dragonCapsule->Description, 500);
		}
		else {
			dragonsInSpace.addVehicle(dragonCapsule, today);
			string description = "Last flown on " + payloadName + " to " + destinations[destination].name + " on " + to_string(today) + ".";
			strcpy(description, dragonCapsule->Description, 500);
		}
	}
	else if (payloadNames[payloadChoice].requiresDragon) {
		string description = "Destroyed on failed " + payloadName + " launch to " + destinations[destination].name + " on " + to_string(today) + ".";
		dragonCapsule->FlightActive = 0;
		strcpy(description, dragonCapsule->Description, 500);
	}
	//Step 17: Create mission description
	string description;
	if (successfulLaunch) {
		if (crewMembers==0) {
			description += "On " + to_string(today) + " a SpaceX ";
			description += falconHeavy ? "Falcon Heavy " : "Falcon 9 ";
			description += "lifted off from " + launchSites[launchSite].description + " carrying " + payloadNames[payloadChoice].supplier + "'s " + payloadName + " payload to " + destinations[destination].name + ".";
		}
		else {
			description += "SpaceX successfully launched " + to_string(crewMembers) + " ";
			description = payloadChoice != 10 ? description + "Astronauts " : description + "Space Tourists ";
			description += "to " + destinations[destination].name + " orbit onboard their ";
			description += falconHeavy ? "Falcon Heavy " : "Falcon 9 ";
			description += " rocket on " + to_string(today) + ".";
		}
		if (payloads.size() > 1) {
			description += " Secondary payloads also included in this flight are: ";
			for (int i = 1; i < payloads.size(); i++) {
				description += payloads[i]->Title;
				if (i == payloads.size() - 1) {
					description += ".";
				}
				else if (payloads.size() > 3 && i == payloads.size() - 2) {
					description += ", and ";
				}
				else {
					description += ", ";
				}
			}
		}
		description += " Complete mission success.";
	}
	else {
		if (crewMembers == 0) {
			description += "On " + to_string(today) + " a SpaceX ";
			description += falconHeavy ? "Falcon Heavy " : "Falcon 9 ";
			description += "exploded seconds after lifting off from " + launchSites[launchSite].description + " carrying " + payloadNames[payloadChoice].supplier + "'s " + payloadName + " payload to " + destinations[destination].name + ".";
			if (payloads.size() > 1) {
				description += " Secondary payloads also included in this flight were: ";
				for (int i = 1; i < payloads.size(); i++) {
					description += payloads[i]->Title;
					if (i == payloads.size() - 1) {
						description += ".";
					}
					else if (payloads.size() > 3 && i == payloads.size() - 2) {
						description += ", and ";
					}
					else {
						description += ", ";
					}
				}
			}
		}
		else {
			description += "Shortly after liftoff, a fireball consumed SpaceX's ";
			description += falconHeavy ? "Falcon Heavy " : "Falcon 9 ";
			description += "rocket carrying " + to_string(crewMembers) + " souls who tragically perished in the explosion. The rocket's intended destination was " + destinations[destination].name + " orbit.";
		}
		description += " Mission failed.";
	}

	strcpy(description, mission->Description, 500);

	return true;
}

void runSimulation() {
	cout << "Starting simulation mode..." << endl;

	highestMissionNumber++;
	highestBoosterNumber++;
	highestCapsuleNumber++;

	default_random_engine e;

	while (currentNumberOfFlights < FLIGHTS_TO_PERFORM) {
		updateHangers();
		if (chanceOutOf30(AVERAGE_FLIGHTS_PER_MONTH, e)) { //Let's go to space today!! :)
			cout << "Generating simulated mission #" << highestMissionNumber << "..." << endl;
			generateMission(e);
			currentNumberOfFlights++;
		}
		today += 1; //Advance time forward by one day
	}
	cout << "Simulation complete." << endl;
}