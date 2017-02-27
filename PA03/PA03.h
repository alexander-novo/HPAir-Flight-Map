#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <regex>

#include "FlightMapV1.h"

// FILENAME MACROS ////////////////////////////////////////////////////////////
#define CITY_FILE "cityFile.txt"
#define FLIGHT_FILE "flightFile.txt"
#define REQUEST_FILE "requestFile.txt"

// FUNCTION PROTOTYPES ////////////////////////////////////////////////////////
void loadCities ( FlightMap& );
void loadFlights ( FlightMap& );
void handleRequests ( FlightMap& );
void printFlightPlan ( std::stack<Flight*> );

// REGULAR EXPRESSIONS ////////////////////////////////////////////////////////

//Used for parsing flight information and requests
//Not only used to verify the correct format, but the capture groups also separate important information

//Los Angeles, Indianapolis	810	50
const std::regex regexFlight ( "^([[:alpha:] ]+),[\t ]([[:alpha:] ]+)[\t ]([[:digit:]]+)[\t ]([[:digit:]]+)$" );
//Indianapolis, San Francisco
const std::regex regexRequest ( "^([[:alpha:] ]+),[\t ]([[:alpha:] ]+)$" );