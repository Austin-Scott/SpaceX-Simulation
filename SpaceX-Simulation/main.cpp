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

//Cmd-line arguments to be used in main()
bool directToDatabase = false;
bool skip = false;
bool soft = false;
bool exclusive = false;
string address, schema, username, password;


void parseCmdLineArgs(int argc, char** argv);
void printHelp(string name);

int main(int argc, char** argv) {
	
	parseCmdLineArgs(argc, argv);
	
	//Add required real world launch and landing sites (must be done before data download)
	cout << "Adding real world Launch and Landing sites..." << endl;
	addRealWorldLaunchSites();
	addRealWorldLandingSites();

	if(!skip)
		getRealData();

	//Add required fictional launch and landing sites (must be done before simulation)
	cout << "Adding fictional Launch and Landing sites..." << endl;
	addFictionalLandingSites();
	addFictionalLaunchSites();

	runSimulation();

	if (directToDatabase)
		updateDatabase(address, schema, username, password, soft);
	if(!directToDatabase || (directToDatabase&&!exclusive))
		writeResultsToFiles();

	cleanAllData();
}

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
 -average_flights_per_month <average_flights>        #Sets average number of Falcon 9 flights per month
 -average_falcon_heavy_flights <average_flights>     #Average number of Falcon Heavy flights per 1000 flights
 -average_successful_launches <average_number>       #Average number of successful launches per 1000 launches
 -average_successful_landings <average_number>       #Average number of successful landings per 1000 landings
 -block_five_max_reflights <number_of_flights>       #Sets how many times a block five core can fly before retirement
 -lesser_block_max_reflights <number_of_flights>     #How many times can a core less than block five fly
 -weight_of_dragon <weight>                          #Sets the dry weight of the Dragon Capsule
 -minimum_payload_weight <weight>                    #Sets the minimum weight a payload needs to be
 -dragon_days_between_flights <days>                 #How long do Dragons wait between flights in days
 -minimum_flight_ready_cores <number_of_cores>       #What is the least number of flight ready cores in the hanger
 -minimum_flight_ready_dragons <number_of_dragons>   #What is the least number of flight ready dragons in the hanger
 -maximum_secondary_payloads <max_payloads>          #Sets max number of secondary payloads that can fly
 -average_flights_with_secondary <average_flights>   #Average number of flights with secondary payloads per 1000
 -dragon_crew_capacity <number_of_crew_members>      #Sets max number of crew members in dragon flight
 -bfs_max_suborbital_flights <number_of_flights>     #Sets the max number of suborbital flights per day
 -bfs_suborbital_flight_chance <chance>              #Sets the chance out of 1000 that a suborbital flight will happen
 -bfs_colony_flights_per_month <number_of_flights>   #Sets the average number of BFR colony missions per month
 -bfr_max_reflights <reflights>                      #Sets the max number of times a BFR can fly before retirement
 -bfr_average_successful_launches <average>          #Sets average number of successful BFR launches per 1000
 -bfr_average_successful_landings <average>          #Sets average number of successful BFR landings per 1000
 -bfs_minimum_payload_mass <payload_mass>            #Sets the minimum payload mass of a BFS payload
 -bfs_maximum_payload_mass <payload_mass>            #Sets the maximum payload mass of a BFS payload
 -bfs_colony_stay_duration <days_at_colony>          #Sets the number of days a BFS will stay at a colony
 -bfs_crew_capacity <crew_members>                   #Sets the maximum number of crew members that can fly on BFS
 -bfr_flight_operational <date_bfr_flights_start>    #Sets the day BFR flights can start. Format: "YYYY-MM-DD"
 -same_day_reflight <chance_out_of_1000>             #Sets the chance a block five booster will fly again on same day
 -do_not_count_suborbital                            #Suborbital flights no longer count towards mission total
 
)";
	cout << options << endl;
}

void parseCmdLineArgs(int argc, char** argv) {
	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-count") == 0) {
			if (argc - i >= 2) {
				i++;
				FLIGHTS_TO_PERFORM = atoi(argv[i]);
			}
			else {
				cout << "Error: Incorrect number of arguments following count. Aborting." << endl;
				printHelp(argv[0]);
				abort();
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
				abort();
			}
		}
		else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
			printHelp(argv[0]);
			abort();
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
		else if (strcmp(argv[i], "-average_flights_per_month") == 0) {
			if (argc - i >= 2) {
				i++;
				AVERAGE_FLIGHTS_PER_MONTH = atoi(argv[i]);
			}
			else {
				cout << "Error: Incorrect number of arguments following argument. Aborting." << endl;
				printHelp(argv[0]);
				abort();
			}
		}
		else if (strcmp(argv[i], "-average_falcon_heavy_flights") == 0) {
			if (argc - i >= 2) {
				i++;
				AVERAGE_FALCON_HEAVY_FLIGHTS_PER_1000 = atoi(argv[i]);
			}
			else {
				cout << "Error: Incorrect number of arguments following argument. Aborting." << endl;
				printHelp(argv[0]);
				abort();
			}
		}
		else if (strcmp(argv[i], "-average_successful_launches") == 0) {
			if (argc - i >= 2) {
				i++;
				AVERAGE_SUCCESSFUL_LAUNCHES_PER_1000 = atoi(argv[i]);
			}
			else {
				cout << "Error: Incorrect number of arguments following argument. Aborting." << endl;
				printHelp(argv[0]);
				abort();
			}
		}
		else if (strcmp(argv[i], "-average_successful_landings") == 0) {
			if (argc - i >= 2) {
				i++;
				AVERAGE_SUCCESSFUL_LANDINGS_PER_1000 = atoi(argv[i]);
			}
			else {
				cout << "Error: Incorrect number of arguments following argument. Aborting." << endl;
				printHelp(argv[0]);
				abort();
			}
		}
		else if (strcmp(argv[i], "-block_five_max_reflights") == 0) {
			if (argc - i >= 2) {
				i++;
				BLOCK_V_MAX_REFLIGHTS = atoi(argv[i]);
			}
			else {
				cout << "Error: Incorrect number of arguments following argument. Aborting." << endl;
				printHelp(argv[0]);
				abort();
			}
		}
		else if (strcmp(argv[i], "-lesser_block_max_reflights") == 0) {
			if (argc - i >= 2) {
				i++;
				LESSER_BLOCK_MAX_REFLIGHTS = atoi(argv[i]);
			}
			else {
				cout << "Error: Incorrect number of arguments following argument. Aborting." << endl;
				printHelp(argv[0]);
				abort();
			}
		}
		else if (strcmp(argv[i], "-weight_of_dragon") == 0) {
			if (argc - i >= 2) {
				i++;
				WEIGHT_OF_DRAGON = atoi(argv[i]);
			}
			else {
				cout << "Error: Incorrect number of arguments following argument. Aborting." << endl;
				printHelp(argv[0]);
				abort();
			}
		}
		else if (strcmp(argv[i], "-minimum_payload_weight") == 0) {
			if (argc - i >= 2) {
				i++;
				MINIMUM_PAYLOAD_WEIGHT = atoi(argv[i]);
			}
			else {
				cout << "Error: Incorrect number of arguments following argument. Aborting." << endl;
				printHelp(argv[0]);
				abort();
			}
		}
		else if (strcmp(argv[i], "-dragon_days_between_flights") == 0) {
			if (argc - i >= 2) {
				i++;
				DRAGON_DAYS_BETWEEN_FLIGHTS = atoi(argv[i]);
			}
			else {
				cout << "Error: Incorrect number of arguments following argument. Aborting." << endl;
				printHelp(argv[0]);
				abort();
			}
		}
		else if (strcmp(argv[i], "-minimum_flight_ready_cores") == 0) {
			if (argc - i >= 2) {
				i++;
				MINIMUM_FLIGHT_READY_CORES = atoi(argv[i]);
			}
			else {
				cout << "Error: Incorrect number of arguments following argument. Aborting." << endl;
				printHelp(argv[0]);
				abort();
			}
		}
		else if (strcmp(argv[i], "-minimum_flight_ready_dragons") == 0) {
			if (argc - i >= 2) {
				i++;
				MINIMUM_FLIGHT_READY_DRAGONS = atoi(argv[i]);
			}
			else {
				cout << "Error: Incorrect number of arguments following argument. Aborting." << endl;
				printHelp(argv[0]);
				abort();
			}
		}
		else if (strcmp(argv[i], "-maximum_secondary_payloads") == 0) {
			if (argc - i >= 2) {
				i++;
				MAXIMUM_SECONDARY_PAYLOADS = atoi(argv[i]);
			}
			else {
				cout << "Error: Incorrect number of arguments following argument. Aborting." << endl;
				printHelp(argv[0]);
				abort();
			}
		}
		else if (strcmp(argv[i], "-average_flights_with_secondary") == 0) {
			if (argc - i >= 2) {
				i++;
				AVERAGE_FLIGHTS_WITH_SECONDARY_PAYLOADS_PER_1000 = atoi(argv[i]);
			}
			else {
				cout << "Error: Incorrect number of arguments following argument. Aborting." << endl;
				printHelp(argv[0]);
				abort();
			}
		}
		else if (strcmp(argv[i], "-dragon_crew_capacity") == 0) {
			if (argc - i >= 2) {
				i++;
				DRAGON_CREW_CAPACITY = atoi(argv[i]);
			}
			else {
				cout << "Error: Incorrect number of arguments following argument. Aborting." << endl;
				printHelp(argv[0]);
				abort();
			}
		}
		else if (strcmp(argv[i], "-bfs_max_suborbital_flights") == 0) {
			if (argc - i >= 2) {
				i++;
				BFS_SUBORBITAL_MAX_FLIGHTS_PER_DAY = atoi(argv[i]);
			}
			else {
				cout << "Error: Incorrect number of arguments following argument. Aborting." << endl;
				printHelp(argv[0]);
				abort();
			}
		}
		else if (strcmp(argv[i], "-bfs_colony_flights_per_month") == 0) {
			if (argc - i >= 2) {
				i++;
				BFS_COLONY_FLIGHTS_PER_MONTH = atoi(argv[i]);
			}
			else {
				cout << "Error: Incorrect number of arguments following argument. Aborting." << endl;
				printHelp(argv[0]);
				abort();
			}
		}
		else if (strcmp(argv[i], "-bfr_max_reflights") == 0) {
			if (argc - i >= 2) {
				i++;
				BFR_MAX_REFLIGHTS = atoi(argv[i]);
			}
			else {
				cout << "Error: Incorrect number of arguments following argument. Aborting." << endl;
				printHelp(argv[0]);
				abort();
			}
		}
		else if (strcmp(argv[i], "-bfr_average_successful_launches") == 0) {
			if (argc - i >= 2) {
				i++;
				BFR_SUCCESSFUL_LAUNCHES_PER_1000 = atoi(argv[i]);
			}
			else {
				cout << "Error: Incorrect number of arguments following argument. Aborting." << endl;
				printHelp(argv[0]);
				abort();
			}
		}
		else if (strcmp(argv[i], "-bfr_average_successful_landings") == 0) {
			if (argc - i >= 2) {
				i++;
				BFR_SUCCESSFUL_LANDINGS_PER_1000 = atoi(argv[i]);
			}
			else {
				cout << "Error: Incorrect number of arguments following argument. Aborting." << endl;
				printHelp(argv[0]);
				abort();
			}
		}
		else if (strcmp(argv[i], "-bfs_minimum_payload_mass") == 0) {
			if (argc - i >= 2) {
				i++;
				BFS_MINIMUM_PAYLOAD_MASS = atoi(argv[i]);
			}
			else {
				cout << "Error: Incorrect number of arguments following argument. Aborting." << endl;
				printHelp(argv[0]);
				abort();
			}
		}
		else if (strcmp(argv[i], "-bfs_maximum_payload_mass") == 0) {
			if (argc - i >= 2) {
				i++;
				BFS_MAXIMUM_PAYLOAD_MASS = atoi(argv[i]);
			}
			else {
				cout << "Error: Incorrect number of arguments following argument. Aborting." << endl;
				printHelp(argv[0]);
				abort();
			}
		}
		else if (strcmp(argv[i], "-bfs_colony_stay_duration") == 0) {
			if (argc - i >= 2) {
				i++;
				BFS_COLONY_STAY_DURATION = atoi(argv[i]);
			}
			else {
				cout << "Error: Incorrect number of arguments following argument. Aborting." << endl;
				printHelp(argv[0]);
				abort();
			}
		}
		else if (strcmp(argv[i], "-bfs_crew_capacity") == 0) {
			if (argc - i >= 2) {
				i++;
				BFS_CREW_CAPACITY = atoi(argv[i]);
			}
			else {
				cout << "Error: Incorrect number of arguments following argument. Aborting." << endl;
				printHelp(argv[0]);
				abort();
			}
		}
		else if (strcmp(argv[i], "-bfr_flight_operational") == 0) {
			if (argc - i >= 2) {
				i++;
				BFR_FLIGHT_OPERATIONAL = Date(string(argv[i]));
			}
			else {
				cout << "Error: Incorrect number of arguments following argument. Aborting." << endl;
				printHelp(argv[0]);
				abort();
			}
		}
		else if (strcmp(argv[i], "-bfs_suborbital_flight_chance") == 0) {
			if (argc - i >= 2) {
				i++;
				BFS_SUBORBITAL_FLIGHT_CHANCE = atoi(argv[i]);
			}
			else {
				cout << "Error: Incorrect number of arguments following argument. Aborting." << endl;
				printHelp(argv[0]);
				abort();
			}
		}
		else if (strcmp(argv[i], "-same_day_reflight") == 0) {
			if (argc - i >= 2) {
				i++;
				SAME_DAY_REFLIGHT = atoi(argv[i]);
			}
			else {
				cout << "Error: Incorrect number of arguments following argument. Aborting." << endl;
				printHelp(argv[0]);
				abort();
			}
		}
		else if (strcmp(argv[i], "-do_not_count_suborbital") == 0) {
			COUNT_SUBORBITAL = false;
		}
		else {
			cout << "Error: Unknown argument \"" << argv[i] << "\". Aborting." << endl;
			printHelp(argv[0]);
			abort();
		}
	}
}