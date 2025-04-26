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
 * @brief Return the amount of people waiting on the floor
 * 
 * @return int 
 */
int Floor::getQueueSize()
{   
    lock_guard<mutex> lock(queueMutex);
    return static_cast<int>(passengerQueue.size());   
}

/**
 * @brief Add new passanger to the queue for the floor
 *        Throw invalid_argument if the passengers desired floor
 *        is not provided or if they are not on current floor
 * 
 * @param newPerson 
 */
void Floor::addToQueue(shared_ptr<Passenger> newPerson)
{   
    lock_guard<mutex> lock(queueMutex);
    if(newPerson->getCurrentLocation() != floorLevel)
    {   throw invalid_argument("Given passenger is not on this floor");   }
    else if(newPerson->getDesiredLocation() == 0)
    {   throw invalid_argument("Given passenger has no desired floor");   }
    else
    {   passengerQueue.push_back(newPerson); }  
}

/**
 * @brief Take the first element in the queue and 
 *         remove the first element from queue
 * 
 * @return std::shared_ptr<Passenger> 
 */
shared_ptr<Passenger> Floor::popFromQueue()
{
    lock_guard<mutex> lock(queueMutex);
    if(passengerQueue.size() != 0)
    {   
        auto passenger = passengerQueue.front();
        passengerQueue.pop_front(); 
        return passenger;
    }
    return nullptr;
}
