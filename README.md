# SpaceX-Simulation
Creates data to populate a simple database for my Databases class. First it uses [Curl](https://curl.haxx.se/) to download a complete SpaceX mission manifest from [this open source REST API](https://github.com/r-spacex/SpaceX-API). Then it parses the downloaded JSON data using the [RapidJSON Library](http://rapidjson.org/). Afterwards, it performs a simulation to generate hypothetical future launch data. Many variables are simulated such as maximum reflights of cores, payload masses limited to launch configuration limits for any destination orbit, and dates that follow real world calendar rules such as everything from months having the correct number of days to properly handling leap years. Finally, the program outputs the conglomeration of real world and simulated data into properly formatted text files for MySQL to read into a database.

## Dependencies
 * Boost 1.66.0 installed to "C:\Program Files\boost_1_66_0"
 * MySQL Connector C++ 1.1.9 installed to "C:\Program Files\MySQL\MySQL Connector C++ 1.1.9"
 * RapidJSON (Included with repository)

Once built, you must have the command line utility Curl downloaded, installed, and added to your PATH variable for the program to collect any real world data. If you don't install Curl the program will run just fine but all the data outputed will come only from the simulation.
