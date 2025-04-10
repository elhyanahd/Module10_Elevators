#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include "Passenger.h"
#include "Simulation.h"

using namespace std;

map<int, Floor> floorList = {};
map<int, Elevator> elevatorList = {};

/**
 * @brief Construct a Simulation object which initializes the 
 *        logger information 
 */
Simulation::Simulation()
{
    //set text file output
    logger = spdlog::basic_logger_mt("sim_logger", "elevator_sim_log.txt");
    //Set Logger pattern and level
    logger->set_pattern("[%Y-%m-%d %H:%M:%S] [%^%l%$] %v"); // timestamp + level
    logger->set_level(spdlog::level::info);
}

/**
 * @brief Loop through untill player seconds start Time is reached
 *        while player start time is not reached, compare current time
 *        with simulation start time.
 * 
 * @param playerTime 
 * @return true 
 * @return false 
 */
bool Simulation::timeDelay(int passengerTime, int passengerFloor)
{
    bool reached = false;

    while(!reached)
    {
        auto currentTime = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::seconds>(currentTime - simulationStart);
        int durationConvert = static_cast<int>(duration.count());
        if(passengerTime == durationConvert)
        {   
            reached = true; 
            //Update log informing of Passenger waiting
            logger->info("New Passenger waiting on floor " + to_string(passengerFloor));
        }
    }

    return reached;
}

/**
 * @brief Keep internal variable of simulation start time.
 *        Update log file with infor on the simulation start time.
 * 
 * @param start 
 */
void Simulation::setStartTime(chrono::time_point<chrono::high_resolution_clock> start)
{
    simulationStart = start;
    logger->info("Elevator Simulation started.");
}

int main()
{
    ifstream input;
    input.open("C:/Desir_Academic_Files/John Hopkins/Object Oriented Programming with C++/Module 10/Mod10_Assignment_Elevators.csv");

    if(!input.is_open())
    {
        cout << "Error opening file.\n";
        return 1;
    }
    else
    {
        string line;
        Simulation sim;
        
        // Skip reading the first line
        getline(input, line);

        // Store simulation start time and update logger
        sim.setStartTime(chrono::high_resolution_clock::now());

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
            while (!sim.timeDelay(passengerValues[0], passengerValues[1])) {}
            
            //check if floor was already added to map
            //if not add the new floor to map
            auto it = floorList.find(passengerValues[1]);
            if(it == floorList.end())
            {  floorList[passengerValues[1]] = Floor(passengerValues[1]); }
            //add new passenger to floor's queue
            floorList[passengerValues[1]].addToQueue(passenger);
        }
        input.close();
    }
    return 0;
}