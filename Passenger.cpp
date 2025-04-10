#include "Passenger.h"
#include <stdexcept>
#include <chrono>
#include <thread>

using namespace std;

/**
 * @brief Construct a Passenger object which sets the private variables
 *        Throw invalid arguments when the given floor parameters are out of bounds
 *        or they are equivalent to each other
 * 
 * @param desiredFloor 
 * @param currentFloor 
 */
Passenger::Passenger(int desiredFloor, int currentFloor, int start)
{
    if(currentFloor < 1 || currentFloor > 100)
    {   throw invalid_argument("Provided current floor location is invalid");   }
    else if(desiredFloor < 1 || desiredFloor > 100) //|| desiredFloor == currentFloor)
    {   throw invalid_argument("Provided desired floor location is invalid");   }
    else
    {
        desired = desiredFloor;
        current = currentFloor;
        startTime = start;
        endTime = 0;
    }
}

/**
 * @brief Reurn the passenger's desired floor location
 * 
 * @return int 
 */
int Passenger::getDesiredLocation() const 
{   return desired; }

/**
 * @brief Reurn the passenger's current floor location
 * 
 * @return int 
 */
int Passenger::getCurrentLocation() const 
{   return current; }

// int Passenger::setEndTime() const
// {
//     auto end = chrono::steady_clock::now();
// }