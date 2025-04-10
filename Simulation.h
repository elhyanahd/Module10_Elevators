#ifndef SIMULATION_H
#define SIMULATION_H

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <map>
#include <chrono>
#include <thread>
#include "Floor.h"
#include "Elevator.h"

class Simulation 
{
    public:
        Simulation();
        bool timeDelay(int passengerTime, int passengerFloor);
        void setStartTime(std::chrono::time_point<std::chrono::high_resolution_clock> start);
        
    protected:
        std::shared_ptr<spdlog::logger> logger;

    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> simulationStart;
        std::chrono::time_point<std::chrono::high_resolution_clock> simulationEnd;
        static std::map<int, Floor> floorList;          //list of floors with information on passengers waiting on each floor
        static std::map<int, Elevator> elevatorList;    //list of elevators used to keep track of dropping/picking up passangers
};

#endif