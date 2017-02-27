#include "FlightMapV1.h"

std::stack<Flight*> FlightMap::convertPathToFlights ( std::stack<City*>& cityPath )
{
    std::stack<Flight*> flightPath;
    City* from, *to;

    while ( cityPath.size() > 1)
    {
        to = cityPath.top ();
        cityPath.pop ();
        from = cityPath.top ();

        for ( Flight& flight : from->flights )
        {
            if ( flight.to == to )
            {
                flightPath.push ( &flight );
                log ( "Flight from " + flight.from->name 
                      + " to " + flight.to->name );
                break;
            }
        }
    }

    return flightPath;
}

void FlightMap::unvisitAll ()
{
    for ( City& city : cities )
    {
        city.visited = false;
    }
}

City* FlightMap::getNextCity ( City* from )
{
    for ( Flight& flight : from->flights )
    {
        if ( !flight.to->visited )
        {
            return flight.to;
        }
    }
    return nullptr;
}

City* FlightMap::addCity ( std::string& name )
{
    City temp;
    temp.name = name;
    cities.push_back ( temp );
    return &cities.back ();
}

City* FlightMap::getCity ( std::string& name )
{
    for ( City& city : cities )
    {
        if ( city.name == name )
        {
            return &city;
        }
    }
    return nullptr;
}

std::stack<Flight*> FlightMap::getPath ( City* from, City* to )
{
    std::stack<City*> cityStack;

    unvisitAll ();

    log ( "\nAttempting to find path from " + from->name + " to " + to->name );

    cityStack.push ( from );
    from->visited = true;

    while ( !cityStack.empty () && ( cityStack.top () != to ) )
    {
        City* nextCity = getNextCity ( cityStack.top() );

        if ( nextCity == nullptr )
        {
            log ( "dead end" );
            cityStack.pop ();
        }
        else
        {
            log ( "Looking at " + nextCity->name );
            cityStack.push ( nextCity );
            nextCity->visited = true;
        }
    }

    if ( cityStack.empty () )
    {
        log ( "Failed to find path" );
    }
    else
    {
        log ( "Found path..." );
    }

    return convertPathToFlights(cityStack);
}


void log ( const std::string& text, bool newLine )
{
#ifdef LOG_FILE
    static std::ofstream logFile ( LOG_FILE );
    logFile << text;
    if ( newLine ) logFile << std::endl;
#endif
}