
#include "Queuer.h"
#include "Passenger.h"
#include "Elevator.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

/**
 * @brief Parse through csv file and record the time new
 *        passengers began waiting, their desired floors,
 *        and current floor level location
 * 
 * @return int 
 */
void PassengerQueuer::beginQueue()
{
    ifstream input;
    input.open("C:/Desir_Academic_Files/John Hopkins/Object Oriented Programming with C++/Module 10/Mod10_Assignment_Elevators.csv");

    //If file does not open, add error message to log
    //return failed output (similar to main)
    if(!input.is_open())
    {   
        logger->addLogMessage("Error opening CSV file. Simulation did not start.");  
        return ;
    }
    else
    {
        logger->addLogMessage("Beginning to Parse CSV file."); 
        string line;
           
        // Skip reading the first line
        getline(input, line);

        //loop through the csv file and store passengers startTime, desired floor, and
        //current floor location. Then queue the passengers up to on their desired floor
        int i = 0;
        while(getline(input, line) && i < 2)
        {   
            //Seperate column by delimeter and store information of new Passanger
            //create new Passanger object with the information
            istringstream iss(line);
            string value;
            vector<int> passengerValues;
            while(getline(iss, value, ','))
            {   passengerValues.push_back(stoi(value)); }

            //store time when queueing started
            queueStart = (queueTime == 0) ? chrono::high_resolution_clock::now() : queueStart;
            
            //time delay wait until passenger start time is reached
            //prior to adding passenger to associated floor queue
            this_thread::sleep_for(chrono::seconds(passengerValues[0] - queueTime));
            
            //update time with start time of latest queued passenger
            queueTime = passengerValues[0];  
            //add queued passenger to their desired floor
            addPassengerToFloor(make_shared<Passenger>(passengerValues[2], passengerValues[1], passengerValues[0], false));

            i++; //TO BE REMOVED
        }
        input.close();

        logger->addLogMessage("Finished parsing CSV file.");
        lock_guard<mutex> lock(parseMutex);
        parsingDone = true;        
    }
}

/**
 * @brief Updated the floorList with the given parameter, addding
 *        passenger to the queue of those waiting on that floor
 *        Also, add new passenger's current floor location to queue for pick Up
 * 
 * @param newPassenger 
 */
void PassengerQueuer::addPassengerToFloor(shared_ptr<Passenger> newPassenger)
{
    lock_guard<mutex> lock(floorMutex);
    int passengerLocation = newPassenger->getCurrentLocation();
    
    // Add the floor object to the map, if it doesn't already exist
    auto& floor = floorList.emplace(passengerLocation, make_shared<Floor>(passengerLocation)).first->second;

    // Add new passenger to the floor's queue
    floor->addToQueue(newPassenger);

    //add passengers floor to pick up request
    addPickUpRequest(passengerLocation);
    logger->addLogMessage("New Passenger waiting on floor " + to_string(passengerLocation));
}

/**
 * @brief Return the time when first Passenger began 
 *         waiting (aka start time)
 * 
 * @return  std::chrono::time_point<std::chrono::high_resolution_clock> 
 */
chrono::time_point<chrono::high_resolution_clock> PassengerQueuer::getStartTime() const
{   return queueStart;  }

/**
 * @brief Return the floor level at the front of the queue.
 *        Remove floor from internal variables that are keeping track
 *        of requested floors
 * 
 * @return int 
 */
int PassengerQueuer::getPickUpRequests()
{
    lock_guard<mutex> lock(requestsMutex);
    int nextFloor = (requestsQueue.empty()) ? 0 : requestsQueue.front();
    if (nextFloor != 0)
    {   
        requestsQueue.pop();
        pickUpRequests.erase(nextFloor);
    }

    return nextFloor;
}

/**
 * @brief Check if floor passenger requested to be picked up
 *        on is already on the list. If not, queue the floorID
 *        and store the floorID in the list for future checks
 * 
 * @param floorID (floor number)
 */
void PassengerQueuer::addPickUpRequest(int floorID)
{   
    lock_guard<mutex> lock(requestsMutex);

    if (pickUpRequests.find(floorID) == pickUpRequests.end() ) 
    {
        requestsQueue.push(floorID);
        pickUpRequests.insert(floorID);
    }
};

/**
 * @brief Retrieve the Floor object that is associated
 *          with the floorID parameter (aka the floor level number)
 * 
 * @param floorID 
 * @return shared_ptr<Floor> 
 */
shared_ptr<Floor> PassengerQueuer::getFloor(int floorID)
{   
    lock_guard<mutex> lock(floorMutex);                         
    return (floorList.find(floorID) != floorList.end()) ? floorList[floorID] : nullptr;
};

/**
 * @brief Check if there are still passengers by checking if there
 *         are passangers still waiting on the floors and if there
 *         are still pick up requests queued
 * 
 * @return true 
 * @return false 
 */
bool PassengerQueuer::noPassengersWaiting()
{
    lock_guard<mutex> lock(floorMutex);
    lock_guard<mutex> lock2(requestsMutex);

    //TO DO: determine if this is applicable
    if(!pickUpRequests.empty())
    {   return false;   }

    for(auto& it : floorList)
    {
        if(it.second->getQueueSize() != 0)
        {  return false;    }
    }

    return true;
}

/**
 * @brief check if file finished parsing.
 * 
 * @return true 
 * @return false 
 */
bool PassengerQueuer::isParseDone()
{
    lock_guard<mutex> lock(parseMutex);
    return parsingDone;
}