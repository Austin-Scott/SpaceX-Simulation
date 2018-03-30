#include <iostream>
#include <fstream>

#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

#include "MySQL.h"
#include "Tables.h"
#include "Hanger.h"
#include "RealData.h"
#include "SimulatedData.h"

using namespace std;

void printHelp(string name) {
	cout << "Proper syntax:" << endl << name << " [-count numberOfMissionsToGenerate] [-database address schema username password]" << endl;
	cout << "Example:" << endl << name << " -count 2000" << endl;
}

int main(int argc, char** argv) {
	//Parse cmdline arguments
	bool directToDatabase = false;
	string address, schema, username, password;
	for (int i = 1; i < argc-1; i++) {
		if (strcmp(argv[i], "-count") == 0) {
			i++;
			FLIGHTS_TO_PERFORM = atoi(argv[i]);
		}
		else if (strcmp(argv[i], "-database") == 0) {
			if (argc - i >= 5) {
				directToDatabase = true;
				i++;
				address = argv[i];
				i++;
				schema = argv[i];
				i++;
				username = argv[i];
				i++;
				password = argv[i];
			}
			else {
				cout << "Error: Incorrect number of strings following database argument. Aborting." << endl;
				printHelp(argv[0]);
				break;
			}
		}
		else {
			cout << "Error: Unknown argument \"" << argv[i] << "\". Aborting." << endl;
			printHelp(argv[0]);
			break;
		}
	}
	getRealData();
	runSimulation();
	if (directToDatabase)
		updateDatabase(address, schema, username, password);
	writeResultsToFiles();
	cleanAllData();
}