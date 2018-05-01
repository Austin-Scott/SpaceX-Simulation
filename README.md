# SpaceX
Space Exploration Technologies Corporation, better known as SpaceX, has been pushing the limits of what was once thought impossible by pursing the vision of making space travel affordable. For many years now they have been propusively landing the first stage boosters of their flagship Falcon 9 rockets and more recently the boosters from their Falcon Heavy. Founder and CEO Elon Musk hopes that one day SpaceX could be used to colonize the Moon and Mars. Musk also hopes that the BFR, or the Big Falcon Rocket, not only will make colonization a reality but it could also be used to support airplane like suborbital passenger services acrossed the globe. This application generates data for a hypothetical future database that tracks Booster, Dragon Capsule, Payload, and Mission information of SpaceX many years from now during the height of their colonization efforts. 

![falconheavylanding](https://media.giphy.com/media/25Ipk4yBFvRmBCuTfc/giphy.gif)

# SpaceX-Simulation
Creates data to populate a simple but non-trivial database for my Databases class. First it uses [Curl](https://curl.haxx.se/) to download a complete SpaceX mission manifest from [this open source REST API](https://github.com/r-spacex/SpaceX-API). Then it parses the downloaded JSON data using the [RapidJSON Library](http://rapidjson.org/). Afterwards, it performs a simulation to generate hypothetical future launch data. Many variables are simulated such as maximum reflights of cores, payload masses limited to launch configuration limits for any destination orbit, and dates that follow real world calendar rules such as everything from months having the correct number of days to properly handling leap years. Finally, the program outputs the conglomeration of real world and simulated data into a database by either directly connecting to the database or by printing out the results into a text file that can be read into the database manually. 

## Relational Model Diagram
Here is an ER diagram of the database that this program generates the data for:

![relationalmodel](https://user-images.githubusercontent.com/12504656/38783723-a41a3a5c-40cb-11e8-9d92-5e0853973aa5.png)

### Dependencies
The following libraries must be installed if you are going to build this application from source:
 * Boost 1.66.0 installed to "C:\Program Files\boost_1_66_0"
 * MySQL Connector C++ 1.1.9 installed to "C:\Program Files\MySQL\MySQL Connector C++ 1.1.9"
 * RapidJSON (Included with repository)

Once built, you must have the command line utility Curl downloaded, installed, and added to your PATH variable for the program to collect any real world data. If you don't install Curl the program will run just fine but all the data outputed will come only from the simulation.

 ### Command line arguments
 This program has many optional command line arguments that you may use to customize the simulation: 
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
 -bfs_colony_flights_per_month <number_of_flights>   #Sets the average number of BFR colony missions per month
 -bfr_max_reflights <reflights>                      #Sets the max number of times a BFR can fly before retirement
 -bfr_average_successful_launches <average>          #Sets average number of successful BFR launches per 1000
 -bfr_average_successful_landings <average>          #Sets average number of successful BFR landings per 1000
 -bfs_minimum_payload_mass <payload_mass>            #Sets the minimum payload mass of a BFS payload
 -bfs_maximum_payload_mass <payload_mass>            #Sets the maximum payload mass of a BFS payload
 -bfs_colony_stay_duration <days_at_colony>          #Sets the number of days a BFS will stay at a colony
 -bfs_crew_capacity <crew_members>                   #Sets the maximum number of crew members that can fly on BFS
 ~~~~

 ### Goals for future versions
 * Add more diversity to the BFR missions
 
Please note that this is a student project and that it is not officially affiliated with SpaceX in any way.
