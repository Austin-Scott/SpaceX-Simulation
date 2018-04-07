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
 This program has a handful of optional command line arguments to be used. 
 ~~~~
 Usage:
  SpaceX-Simulation [options]
 Options:
  -h | --help                                         #Prints help text and aborts program
  -count <number_of_missions_to_generate>             #Sets the number of simulated missions to generate
  -skip                                               #Skip manifest download. Only performs simulation.
  -database <address> <schema> <username> <password>  #Loads data directly into the specified database
  -soft                                               #If loading into database, drop only tuples not tables
  -exclusive                                          #If loading into database, do so exclusively. Skip txt files.
 ~~~~

 ### Goals for future versions
 * Add more diversity to all the generated descriptions
 * Add BFR colony missions to the simulation
 * Add more payload types to reduce repitition in the generated missions
