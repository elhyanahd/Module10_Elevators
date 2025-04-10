#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include "Passenger.h"
#include "Simulation.h"
#include "SimLogger.h"


#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

using namespace std;

map<int, Elevator> elevatorList = {};
extern SimLogger logger;

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
            logger.addLogMessage("New Passenger waiting on floor " + to_string(passengerFloor));
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
    logger.addLogMessage("Elevator Simulation started.");
}

int main() 
{
    //
    Simulation sim;
    sim.setStartTime(std::chrono::high_resolution_clock::now());
    // PassengerQueuer queuer;
    
    // std::thread(SimLogger());

    return 0;
}