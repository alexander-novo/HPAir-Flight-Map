#include <iostream>
#include <fstream>
#include <string>
#include <regex>

#include "FlightMapV1.h"

#define CITY_FILE "cityFile.txt"
#define FLIGHT_FILE "flightFile.txt"
#define REQUEST_FILE "requestFile.txt"

void loadCities ( FlightMap& );
void loadFlights ( FlightMap& );
void handleRequests ( FlightMap& );
void printFlightPlan ( std::stack<Flight*> );

//Used for parsing flight information and requests
//Not only used to verify the correct format, but the capture groups also separate important information
const std::regex regexFlight ( "^([[:alpha:] ]+),[\t ]([[:alpha:] ]+)[\t ]([[:digit:]]+)[\t ]([[:digit:]]+)$" );
const std::regex regexRequest ( "^([[:alpha:] ]+),[\t ]([[:alpha:] ]+)$" );

int main ()
{
    FlightMap map;

    loadCities ( map );
    loadFlights ( map );
    handleRequests ( map );

}

void loadCities ( FlightMap& map )
{
    std::ifstream inFile ( CITY_FILE );
    std::string name;

    while ( inFile.peek () != EOF )
    {
        //Every line should just be a city name, so don't worry about parsing
        std::getline ( inFile, name );
        map.addCity ( name );
        log ( "Adding City: " + name );
    }
}

void loadFlights ( FlightMap& map )
{
    std::ifstream inFile ( FLIGHT_FILE );
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

void handleRequests ( FlightMap& map )
{
    std::ifstream inFile ( REQUEST_FILE );
    std::string request; //Used to load one line at a time for parsing
    std::smatch match; //Used to hold the parsed information
    std::stack<Flight*> flightPath;
    City* from, *to;

    //Read until the end of the file
    while ( inFile.peek () != EOF )
    {
        std::getline ( inFile, request );
        std::regex_match ( request, match, regexRequest );

        if ( !match.size () )
        {
            log ( "ERROR Failed to parse request: " + request );
            continue;
        }

        std::cout << "Request is to fly from " << match [ 1 ].str ()
            << " to " << match [ 2 ].str () << std::endl;

        from = map.getCity ( match [ 1 ].str () );
        to = map.getCity ( match [ 2 ].str () );

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

