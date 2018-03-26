#include <iostream>
#include <fstream>

#include "Tables.h"
#include "Hanger.h"
#include "RealData.h"
#include "SimulatedData.h"

using namespace std;

int main() {
	getRealData();
	runSimulation();
	writeResultsToFiles();
	cleanAllData();
}