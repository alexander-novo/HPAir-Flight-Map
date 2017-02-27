#pragma once

#include <string>
#include <vector>
#include <stack>

#define LOG_FILE "log.txt"

#ifdef LOG_FILE
#include <fstream>
#endif

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

void log ( const std::string&, bool = true );