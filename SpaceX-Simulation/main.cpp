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

int main() {
	getRealData();
	runSimulation();
	writeResultsToFiles();
	cleanAllData();
}