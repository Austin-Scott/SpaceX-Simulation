#pragma once
#include <random>
#include <time.h>

const int maxSimulatedFlights = 100;
const int block5MaxReflights = 12;

const int flightChancePerDay = 30;
const int falconHeavyFlightChance = 5;
const int launchSuccessChance = 97;
const int landingSuccessChance = 93;



const int refurbishmentTimeInDays = 10;

int currentNumOfFlights = 0;

struct Destination {
	string name;
	int max9Payload;
	int maxHPayload;
};
const int numOfDestinations = 3;
Destination destinations[] = { {"leo",22800,63800}, {"gto", 8300, 26700}, {"mars", 4020, 16800} };

struct PayloadName {
	string name;
	string supplier;
	int suffix;
};
const int numOfPayloadNames=3;
PayloadName payloadNames[] = { {"Awesome", "Everything is awesome inc.", 1}, {"Cool", "Freeze Breeze Corporation", 1}, {"Not a spy sat", "Not the US government", 1} };

Date today(endDate);

string getRandomDescription() {
	return executeSystemCommand("curl -k -s \"https://loripsum.net/api/1/short/prude/plaintext\"");
}

void updateHangers() {
	flightReadyFalcon9s.addBoosters(falcon9Refurbishment.getBoostersOlderThan(today, refurbishmentTimeInDays), today);
	flightReadyFalconHeavySideBoosters.addBoosters(falconHeavySideBoosterRefurbishment.getBoostersOlderThan(today, refurbishmentTimeInDays), today);
}

void runSimulation() {
	highestMissionNumber++;
	highestBoosterNumber++;

	default_random_engine e;
	uniform_int_distribution<int> perc(0, 100);
	uniform_int_distribution<int> destinationChoice(0, numOfDestinations);
	uniform_int_distribution<int> payloadChoice(0, numOfPayloadNames);

	while (currentNumOfFlights < maxSimulatedFlights) {
		if (perc(e) < flightChancePerDay) { //We are going to try to go to space today!!!
			int destination = destinationChoice(e);
			if (perc(e) < falconHeavyFlightChance) { //Lets fly on Falcon Heavy!

			}
			else { //Let's fly on Falcon 9!

			}
			currentNumOfFlights++;
		}
		updateHangers();
		today += 1;
	}
}