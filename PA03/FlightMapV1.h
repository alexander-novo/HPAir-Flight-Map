// CLASS HEADER INFORMATION ///////////////////////////////////////////////////
/**
  * @file FlightMapV1.h
  *
  * @brief Header for FlightMap implementation
  *
  * @details FlightMap class declaration, as well as City and Flight structs
  *          and log function used by FlightMap
  *
  * @version 1.00
  *          Alexander Novotny
  *          First version
  *
  */

#pragma once

// INCLUDES ///////////////////////////////////////////////////////////////////
#include <string>
#include <vector>
#include <stack>

// PRECOMPILER MACROS /////////////////////////////////////////////////////////
#define LOG_FILE "log.txt"

#ifdef LOG_FILE
#include <fstream>
#endif

// STRUCTS ////////////////////////////////////////////////////////////////////
struct Flight;

struct City
{
    std::string name;
    std::vector<Flight> flights;
    bool visited;
};

struct Flight
{
    City* from;
    City* to;
    unsigned int id;
    unsigned int cost;
};

// CLASS DEFINITION ///////////////////////////////////////////////////////////
class FlightMap
{
    private:
        std::vector<City> cities;

        static std::stack<Flight*> convertPathToFlights ( std::stack<City*>& cityPath );

        void unvisitAll ();
        City* getNextCity ( City* from );

    public:
        City* addCity ( std::string& name );
        City* getCity ( std::string& name );

        std::stack<Flight*> getPath ( City* from, City* to );
};

// LOG FUNCTION ///////////////////////////////////////////////////////////////
void log ( const std::string&, bool = true );