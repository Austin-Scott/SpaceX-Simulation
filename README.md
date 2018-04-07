# SpaceX-Simulation
Creates data to populate a simple database for my Databases class. First it uses [Curl](https://curl.haxx.se/) to download a complete SpaceX mission manifest from [this open source REST API](https://github.com/r-spacex/SpaceX-API). Then it parses the downloaded JSON data using the [RapidJSON Library](http://rapidjson.org/). Afterwards, it performs a simulation to generate hypothetical future launch data. Many variables are simulated such as maximum reflights of cores, payload masses limited to launch configuration limits for any destination orbit, and dates that follow real world calendar rules such as everything from months having the correct number of days to properly handling leap years. Finally, the program outputs the conglomeration of real world and simulated data into a database by either directly connecting to the database or by printing out the results into a text file that can be read into the database manually. 

## Entity Relation Diagram
![entityrelationdiagram](https://user-images.githubusercontent.com/12504656/38458013-021c88a4-3a5e-11e8-926c-0671244fb9c2.png)

### Dependencies
 * Boost 1.66.0 installed to "C:\Program Files\boost_1_66_0"
 * MySQL Connector C++ 1.1.9 installed to "C:\Program Files\MySQL\MySQL Connector C++ 1.1.9"
 * RapidJSON (Included with repository)

Once built, you must have the command line utility Curl downloaded, installed, and added to your PATH variable for the program to collect any real world data. If you don't install Curl the program will run just fine but all the data outputed will come only from the simulation.

 ### Command line arguments
 This program has a couple of optional command line arguments to be used. 
  * -count can be used to specify how many missions you want the program to generate. The following example tells the program to generate 2500 missions: "SpaceX-Simulation -count 2500"
  * -database can be used to connect directly to the database where you want the results to be stored. Immediately following this switch you must specify the address of the database, the schema to be used, the username, and the password exactly in that order. Here is an example usage: "SpaceX-Simulation -database tcp://127.0.0.1:3306 db1 root password"

 ### Goals for future versions
 * Add more quantitative attributes to the tables
 * Add more diversity to all the generated descriptions
 * Add BFR colony missions to the simulation
 * Add more payload types to reduce repitition in the generated missions
