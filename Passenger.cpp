#include "Passenger.h"
#include <stdexcept>
#include <chrono>
#include <thread>
#include <sstream>
#include <iostream>
#include <string>
#include <fstream>
#include "SimLogger.h"

using namespace std;

extern SimLogger logger = SimLogger();                //global simulation logger object
//map<int, Floor> floorList = {};                //global map of all floors and passengers waiting on each floor

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

int PassengerQueuer::Queuer()
{
    ifstream input;
    input.open("C:/Desir_Academic_Files/John Hopkins/Object Oriented Programming with C++/Module 10/Mod10_Assignment_Elevators.csv");

    //If file does not open, add error message to log
    //return failed output (similar to main)
    if(!input.is_open())
    {   
        logger.addLogMessage("Error opening CSV file. Simulation did not start.");  
        return 1;
    }
    else
    {
        string line;
           
        // Skip reading the first line
        getline(input, line);

        // // Store simulation start time and update logger
        // simulationObject.setStartTime(chrono::high_resolution_clock::now());

        //loop through the csv file and store passengers startTime, desired floor, and
        //current floor location. Then queue the passengers up to on their desired floor
        //while(getline(input, line))
        getline(input, line);
        {   
            //Seperate column by delimeter and store information of new Passanger
            //create new Passanger object with the information
            istringstream iss(line);
            string value;
            vector<int> passengerValues;
            while(getline(iss, value, ','))
            {   passengerValues.push_back(stoi(value)); }
            
            Passenger passenger(passengerValues[2], passengerValues[1], passengerValues[0]);

            //time delay loop until passenger start time is reached
            //prior to adding to associated floor queue
            //while (!simulationObject.timeDelay(passengerValues[0], passengerValues[1])) {}
            
            //check if floor was already added to map
            //if not add the new floor to map
            // auto it = floorList.find(passengerValues[1]);
            // if(it == floorList.end())
            // {  floorList[passengerValues[1]] = Floor(passengerValues[1]); }
            //add new passenger to floor's queue
            //floorList[passengerValues[1]].addToQueue(passenger);
        }
        input.close();
    }
    return 0;
}