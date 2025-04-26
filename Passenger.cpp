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
Passenger::Passenger(int desiredFloor, int currentFloor, chrono::time_point<std::chrono::high_resolution_clock> start)
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
 * @brief Set internal variable to given parameter
 *        which will indicate when passenger started
 *        riding the elevator
 * 
 * @param riding 
 * @param enterTime 
 */
void Passenger::setElevatorInfo(chrono::time_point<chrono::high_resolution_clock> enterTime)
{   ridingTime = enterTime; }

/**
 * @brief Set internal variable to given parameter
 *        which will indicate whether  when 
 *        passenger stopped riding the elevator
 * 
 * @param exitTime 
 */
void Passenger::setReachedInfo(std::chrono::time_point<std::chrono::high_resolution_clock> exitTime)
{   endTime = exitTime; }

/**
 * @brief Return the wait time based on the startTime (when the Passenger began waiting)
 *        and the ride time (when the Passenger entered the elevator)
 * 
 * @return int 
 */
int Passenger::getWaitTime() const
{   
    auto time = ridingTime - startTime;
    auto time_s = chrono::duration_cast<chrono::seconds>(time).count();  
    return static_cast<int>(time_s);
}

/**
 * @brief Return the travel time based on the endTime (when the Passenger exited the elevator)
 *        and the ride time (when the Passenger entered the elevator)
 * 
 * @return int 
 */
int Passenger::getTravelTime() const
{   
    auto time = endTime - ridingTime;  
    auto time_s = chrono::duration_cast<chrono::seconds>(time).count();  
    return static_cast<int>(time_s);
}