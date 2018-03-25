#pragma once
#include <random>
#include <time.h>

#define FLIGHTS_TO_PERFORM 1000
#define AVERAGE_FLIGHTS_PER_MONTH 5
#define AVERAGE_FALCON_HEAVY_FLIGHTS_PER_1000 50
#define AVERAGE_SUCCESSFUL_LAUNCHES_PER_1000 989
#define AVERAGE_SUCCESSFUL_LANDINGS_PER_1000 935
#define BLOCK_V_MAX_REFLIGHTS 12
#define LESSER_BLOCK_MAX_REFLIGHTS 2
#define WEIGHT_OF_DRAGON 4200


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
};
const int numOfPayloadNames=23;
PayloadName payloadNames[] = { {"CRS-", "NASA (Cargo Resupply Services)", true, 2, 14}, {"CCS-", "NASA (Commercial Crew Services)", true, 2, 1}, {"Red Dragon-", "SpaceX", true, 4, 1}, {"SES-", "SES", false, 3, 12}, {"NROL-", "NROL", false, -1, 77}, {"Iridium NEXT ", "Iridium Communications", false, 1, 5}, {"STARLINK-", "SpaceX", false, 0, 1}, {"GovSat-", "GovSat", false, 3, 2}, {"X37B OTV-", "USAF", false, -1, 6}, {"Inmarsat-", "Inmarsat", false, 3, 6}, {"Space Tourism-", "SpaceX", true, -1, 1}, {"FormoSat-", "NSPO (Taiwan)", false, 6, 6 }, {"EchoStar ", "EchoStar", false, 3, 106}, {"BulgariaSat-", "Bulgaria Sat", false, 3, 2}, {"Amos-", "Spacecom", false, 3, 7}, {"JCSAT-", "SKY Perfect JCSAT Group", false, 3, 17}, {"KoreaSat ", "KT Corporation", false, 3, 6}, {"Thaicom ", "Thaicom", false, 3, 9}, {"AsiaSat ", "AsiaSat", false, 0, 9}, {"Jason-", "NASA", false, 1, 4}, {"Eutelsat ", "Eutelsat", false, 3, 118}, {"Microsat-", "SpaceX", false, 0, 3}, {"Cubesats ", "NRO", false, 0, 2} };



void updateHangers() {
	
}

void runSimulation() {
	highestMissionNumber++;
	highestBoosterNumber++;

	default_random_engine e;

	while (currentNumberOfFlights < FLIGHTS_TO_PERFORM) {
		
		updateHangers();
		today += 1; //Advance time forward by one day
	}
}