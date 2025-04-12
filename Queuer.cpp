
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
            Passenger passenger(passengerValues[2], passengerValues[1], passengerValues[0], false);

            //store time when queueing started
            queueStart = (queueTime == 0) ? chrono::high_resolution_clock::now() : queueStart;
            
            //time delay wait until passenger start time is reached
            //prior to adding passenger to associated floor queue
            this_thread::sleep_for(chrono::seconds(passengerValues[0] - queueTime));
            logger->addLogMessage("New Passenger waiting on floor " + to_string(passengerValues[1]));
            //update time with start time of latest queued passenger
            queueTime = passengerValues[0];                

            // Add the floor to the map, if it doesn't already exist
            auto& floor = floorList.emplace(passengerValues[1], make_shared<Floor>(Floor(passengerValues[1]))).first->second;

            // Add new passenger to the floor's queue and private variable
            floor->addToQueue(make_shared<Passenger>(passenger));
            passengerList.push(make_shared<Passenger>(passenger));

            i++;
        }
        input.close();
    }
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
 * @brief Get  list of latest passenger was queued.
 * 
 * @return int 
 */
queue<shared_ptr<Passenger>> PassengerQueuer::getQueue()
{   return passengerList;  };

/**
 * @brief Get list of passengers waiting on floors.
 * 
 * @return int 
 */
map<int, shared_ptr<Floor>> PassengerQueuer::getFloorList()
{   return floorList;  };

void ElevatorController::beginLoop()
{
    bool simulationRun = true;
    shared_ptr<Elevator> elevatorA = make_shared<Elevator>("A", logger);

    while(simulationRun)
    {
        //check if passenger was added to waiting queue
        if(queued->getQueue().size() > 0)
        {
            //Store next queued passanger, and remove from queue 
            shared_ptr<Passenger> person = queued->getQueue().front();
            queued->getQueue().pop();

            //check what elevators are available to pick up unit
            //if elevator is stopped, empty, or ... move to the
            // passengers current floor location.
            if(elevatorA->showStatus() == Movement::STOPPED)
            {
                elevatorA->moveFloors(person->getCurrentLocation(), queued->getFloorList());
            }
        }
        this_thread::sleep_for(chrono::seconds(1));     //loop every second
    }
}

