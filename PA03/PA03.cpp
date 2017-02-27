// PROGRAM DRIVER INFORMATION /////////////////////////////////////////////////
/** 
  * @file PA03.cpp
  *
  * @brief Program driver implementation file
  *
  * @details Reads in city and flight information, and then processes flight requests
  *
  * @version 1.00
  *          Alexander Novotny
  *          First version
  *
  */

// PROGRAM DRIVER HEADER //////////////////////////////////////////////////////
#include "PA03.h"

// MAIN FUNCTION //////////////////////////////////////////////////////////////
int main ()
{
    FlightMap map;

    loadCities ( map, CITY_FILE );
    loadFlights ( map, FLIGHT_FILE );
    handleRequests ( map, REQUEST_FILE );

}

/**
  * @brief Loads a list of cities from fileName and adds them to map
  *
  * @pre fileName contains the name of a valid file
  *
  * @post map will be filled with any new cities found in the file
  *
  * @param[in] map
  *            The FlightMap to add cities to
  *
  * @param[in] fileName
  *            The name of the file to load cities from
  *
  */
void loadCities ( FlightMap& map, const std::string& fileName )
{
    std::ifstream inFile ( fileName );
    std::string name;

    while ( inFile.peek () != EOF )
    {
        //Every line should just be a city name, so don't worry about parsing
        std::getline ( inFile, name );
        map.addCity ( name );
        log ( "Adding City: " + name );
    }
}

/**
  * @brief Loads a list of flights from fileName and adds them to map
  *
  * @pre fileName contains the name of a valid file
  *
  * @post Adds any parsed flights from fileName into the cities contained in map
  *
  * @param[in] map
  *            The FlightMap to add flights to
  *
  * @param[in] fileName
  *            The name of the file to load cities from
  *
  */
void loadFlights ( FlightMap& map, const std::string& fileName )
{
    std::ifstream inFile ( fileName );
    std::string flightLine; //Used to load one line at a time for parsing
    std::smatch match; //Used to hold the parsed information
    Flight flight;

    City* from, *to;

    //Read until the end of the file
    while ( inFile.peek () != EOF )
    {
        //Read a line and parse it
        std::getline ( inFile, flightLine );
        std::regex_match ( flightLine, match, regexFlight );

        //The line doesn't match the required specifications
        if ( !match.size () )
        {
            log ( "ERROR Failed to add flight: " + flightLine );
            continue;
        }

        from = map.getCity ( match [ 1 ].str () );
        to = map.getCity ( match [ 2 ].str () );

        //Cities could not be found
        if ( from == nullptr )
        {
            log ( "ERROR Failed to add flight: Could not find city \"" 
                  + match [ 1 ].str () + "\"" );
            continue;
        }
        if ( to == nullptr )
        {
            log ( "ERROR Failed to add flight: Could not find city \""
                  + match [ 2 ].str () + "\"" );
            continue;
        }

        //Add flight to city's flight list
        flight.id = stoi ( match [ 3 ].str () );
        flight.cost = stoi ( match [ 4 ].str () );
        flight.from = from;
        flight.to = to;

        from->flights.push_back ( flight );

        log ( "Adding flight " + std::to_string ( flight.id )
              + " from " + from->name + " to " + to->name
              + " for $" + std::to_string ( flight.cost ) );
    }
}

/**
  * @brief Loads a list of requests from fileName and then processes them
  *
  * @details Loads a list of requests from fileName, then attempts to find a
  *          path for them using map.getPath() and either tells the client that
  *          HPAir doesn't fly there or gives them their flight plan
  *
  * @pre fileName contains the name of a valid file
  *
  * @pre map contains a list of cities filled with flights
  *
  * @param[in] map
  *            The FlightMap to get cities and flights from
  *
  * @param[in] fileName
  *            The name of the file to load cities from
  *
  */
void handleRequests ( FlightMap& map, const std::string& fileName )
{
    std::ifstream inFile ( fileName );
    std::string request; //Used to load one line at a time for parsing
    std::smatch match; //Used to hold the parsed information
    std::stack<Flight*> flightPath;
    City* from, *to;

    //Read until the end of the file
    while ( inFile.peek () != EOF )
    {
        //Read and parse one line at a time
        std::getline ( inFile, request );
        std::regex_match ( request, match, regexRequest );

        //The line doesn't match the required specifications
        if ( !match.size () )
        {
            log ( "ERROR Failed to parse request: " + request );
            continue;
        }

        std::cout << "Request is to fly from " << match [ 1 ].str ()
            << " to " << match [ 2 ].str () << std::endl;

        from = map.getCity ( match [ 1 ].str () );
        to = map.getCity ( match [ 2 ].str () );

        //Cities could not be found
        if ( from == nullptr )
        {
            log ( "ERROR Failed to fulfill request: Could not find city \"" 
                  + match [ 1 ].str () + "\"" );
            std::cout << "Sorry, HPAir does not service " << match [ 1 ].str ();
            continue;
        }
        if ( to == nullptr )
        {
            log ( "ERROR Failed to fulfill request: Could not find city \"" 
                  + match [ 2 ].str () + "\"" );
            std::cout << "Sorry, HPAir does not service " << match [ 2 ].str ();
            continue;
        }

        //See if we can fly this request
        //Then either notify the client that we can't or print their flight plan
        flightPath = map.getPath ( from, to );
        if ( flightPath.empty () )
        {
            std::cout << "Sorry - HPAir does not fly from " << from->name 
                      << " to " << to->name << std::endl << std::endl;
        }
        else
        {
            printFlightPlan ( flightPath );
        }
    }
}

/**
  * @brief Takes a flight plan and prints it on screen
  *
  * @pre flightPlan must not be empty
  *
  * @param[in] flightPlan
  *            A stack of flights in order of first to last from top to bottom.
  *
  * @param[in] fileName
  *            The name of the file to load cities from
  *
  */
void printFlightPlan ( std::stack<Flight*> flightPlan )
{
    Flight* next;
    unsigned int total = 0;
    while ( !flightPlan.empty () )
    {
        next = flightPlan.top ();
        flightPlan.pop ();

        std::cout << "Flight #" << next->id << " from " << next->from->name 
                  << " to " << next->to->name 
                  << "\tCost: $" << next->cost << std::endl;

        total += next->cost;
    }

    std::cout << "Total Cost ............... $" << total 
              << std::endl << std::endl;
}

