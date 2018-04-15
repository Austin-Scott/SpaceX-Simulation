#pragma once
#include "Utility.h"



Date BFR_FLIGHT_OPERATIONAL = Date("2020-12-26");


struct BFRPlatform {
	LaunchSite* launchSite;
	LandingSite* landingSite;

	Hanger<Dragon> BFSs;
	Hanger<Booster> BFRs;

	BFRPlatform(string SiteID, string Name, string Location) {
		launchSite = createLaunchSite(SiteID + "_lch", Name + " Launch Platform", Location, 1, 1);
		landingSite = createLandingSite(SiteID + "_lnd", Name + " Landing Platform", 1);
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
}

struct ExtraPlanetaryPad {
	LaunchSite* launchSite;
	string destinationOrbit;
	ExtraPlanetaryPad(string SiteID, string Name, string Location, string orbit) {
		launchSite = createLaunchSite(SiteID+"_lch", Name, Location, 1, 1);
		destinationOrbit = orbit;
	}
};

vector<ExtraPlanetaryPad> extraPlanetaryPads;
void addExtraPlanetaryBFRPads() {
	extraPlanetaryPads.emplace_back("mba", "Moon Base Alpha", "Moon, East Mare Imbrium, Archimedes Crater", "Moon");
	extraPlanetaryPads.emplace_back("mrs", "Elon City", "Mars, Equatorial Highlands, Elysium Mons", "Mars");
}

void planetaryBFRFlight() {

}

void extraPlanetaryBFRFlight() {

}

void BFRFlights() {

}