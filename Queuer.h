#ifndef QUEUER_H
#define QUEUER_H

#include <map>
#include <memory>
#include <chrono>
#include <thread>
#include <queue>
#include "SimLogger.h"
#include "Floor.h"

class PassengerQueuer
{
    public:
        PassengerQueuer(std::shared_ptr<SimLogger> logger) : logger(logger), queueTime(0) {};
        void beginQueue();
        std::chrono::time_point<std::chrono::high_resolution_clock> getStartTime() const;
        std::queue<std::shared_ptr<Passenger>> getQueue();
        std::map<int, std::shared_ptr<Floor>> getFloorList();

    private:
        std::shared_ptr<SimLogger> logger;          //variable to access logger class
        std::chrono::time_point<std::chrono::high_resolution_clock> queueStart;                             //second when last passenger was queued
        int queueTime;
        std::map<int, std::shared_ptr<Floor>> floorList;
        std::queue<std::shared_ptr<Passenger>> passengerList;
};

class ElevatorController
{
    public:
        ElevatorController(std::shared_ptr<SimLogger> logger, std::shared_ptr<PassengerQueuer> queued) : logger(logger), queued(queued), queueEnd(0) {};
        void beginLoop();

    private:
        std::shared_ptr<SimLogger> logger;              //variable to access logger class
        std::shared_ptr<PassengerQueuer> queued;        //variable to access PassengerQueue class
        int queueEnd;                                   //second when last passenger was queued
        //std::map<int, std::shared_ptr<Floor>> floorList;
};
#endif