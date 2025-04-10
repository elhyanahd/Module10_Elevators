#ifndef SIMULATION_H
#define SIMULATION_H

#include <map>
#include <chrono>
#include <thread>
//#include "Floor.h"
#include "Elevator.h"

class Simulation 
{
    public:
        Simulation() = default;
        bool timeDelay(int passengerTime, int passengerFloor);
        void setStartTime(std::chrono::time_point<std::chrono::high_resolution_clock> start);

    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> simulationStart;
        std::chrono::time_point<std::chrono::high_resolution_clock> simulationEnd;
        static std::map<int, Elevator> elevatorList;    //list of elevators used to keep track of dropping/picking up passangers
};

#endif