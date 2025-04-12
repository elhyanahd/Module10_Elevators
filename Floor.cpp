#include "Floor.h"
#include <stdexcept>

using namespace std;

/**
 * @brief Construct a Floor object which sets specific floor level ID
 *        Throw invalid_argument if level is outside of min and max
 * 
 * @param level 
 */
Floor::Floor(int level)
{   
    if (level < 1 || level > 100)
    {   throw invalid_argument("Level given is outside of bounds 1 to 100");   }
    else
    {   floorLevel = level; } 
}

/**
 * @brief Construct a new Floor object which sets internal variables
 *        Throw invalid_argument if level is outside of min and max
 * 
 * @param level 
 * @param newQueue 
 */
Floor::Floor(int level, list<shared_ptr<Passenger>> newQueue)
{
    if (level < 1 || level > 100)
    {   throw invalid_argument("Level given is outside of bounds 1 to 100");   }
    else
    {  
        floorLevel = level;
        passengerQueue = newQueue;
    }
}

/**
 * @brief Return the amound of people waiting on the floor
 * 
 * @return int 
 */
int Floor::getQueueSize() const
{   return static_cast<int>(passengerQueue.size());   }

/**
 * @brief Add new passanger to the queue for the floor
 *        Throw invalid_argument if the passengers desired floor
 *        is not provided or if they are not on current floor
 * 
 * @param newPerson 
 */
void Floor::addToQueue(shared_ptr<Passenger> newPerson)
{   
    if(newPerson->getCurrentLocation() != floorLevel)
    {   throw invalid_argument("Given passenger is not on this floor");   }
    else if(newPerson->getDesiredLocation() == 0)
    {   throw invalid_argument("Given passenger has no desired floor");   }
    else
    {   passengerQueue.push_back(newPerson); }  
}

/**
 * @brief Return the floor level of this object
 * 
 * @return int 
 */
int Floor::getFloorLevel() const
{   return floorLevel;  }

/**
 * @brief Returns the list of passangers waiting in that floor
 * 
 * @return list<shared_ptr<Passenger>> 
 */
list<shared_ptr<Passenger>> Floor::getWaitingPassengers()
{   return passengerQueue;  }