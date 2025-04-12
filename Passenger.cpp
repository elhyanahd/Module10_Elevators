#include "Passenger.h"
#include <stdexcept>
#include <iostream>

using namespace std;

/**
 * @brief Construct a Passenger object which sets the private variables
 *        Throw invalid arguments when the given floor parameters are out of bounds
 *        or they are equivalent to each other
 * 
 * @param desiredFloor 
 * @param currentFloor 
 */
Passenger::Passenger(int desiredFloor, int currentFloor, int start, bool inside)
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
        inElevator = inside;
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

/**
 * @brief Return whether the passenger is in the 
 *        elevator or not
 * 
 * @return true 
 * @return false 
 */
bool Passenger::checkIfRiding() const
{   return inElevator;  }

/**
 * @brief Set internal variable to given parameter 
 *        which will indicate whether the passenger is 
 *        riding the elevator
 * 
 * @param riding 
 */
void Passenger::setElevatorCheck(const bool riding)
{   inElevator = riding;    }
