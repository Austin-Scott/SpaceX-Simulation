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
	cout << "\nDescription: \n Retrieves and/or generates data to populate a simple SpaceX themed database." << endl;
	cout << "Usage: \n " << name << " [options]" << endl;
	const string options =
R"(
Options:
 -h | --help                                         #Prints help text and aborts program
 -count <number_of_missions_to_generate>             #Sets the number of simulated missions to generate
 -skip                                               #Skip manifest download. Only performs simulation.
 -database <address> <schema> <username> <password>  #Loads data directly into the specified database
 -soft                                               #If loading into database, drop only tuples not tables
 -exclusive                                          #If loading into database, do so exclusively. Skip txt files.
)";
	cout << options << endl;
}

int main(int argc, char** argv) {
	//Parse cmdline arguments
	bool directToDatabase = false;
	bool skip = false;
	bool soft = false;
	bool exclusive = false;
	string address, schema, username, password;
	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-count") == 0) {
			if (argc - i >= 2) {
				i++;
				FLIGHTS_TO_PERFORM = atoi(argv[i]);
			}
			else {
				cout << "Error: Incorrect number of arguments following count. Aborting." << endl;
				printHelp(argv[0]);
				return 0;
			}
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
				return 0;
			}
		}
		else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
			printHelp(argv[0]);
			return 0;
		}
		else if (strcmp(argv[i], "-skip") == 0) {
			skip = true;
		}
		else if (strcmp(argv[i], "-soft") == 0) {
			soft = true;
		}
		else if (strcmp(argv[i], "-exclusive") == 0) {
			exclusive = true;
		}
		else {
			cout << "Error: Unknown argument \"" << argv[i] << "\". Aborting." << endl;
			printHelp(argv[0]);
			return 0;
		}
	}
	if(!skip)
		getRealData();
	runSimulation();
	if (directToDatabase)
		updateDatabase(address, schema, username, password, soft);
	if(!directToDatabase || (directToDatabase&&!exclusive))
		writeResultsToFiles();
	cleanAllData();
}