#ifndef QUEUER_H
#define QUEUER_H

#include <map>
#include <memory>
#include <chrono>
#include <thread>
#include <list>
#include <mutex>
#include <set>
#include "SimLogger.h"
#include "Floor.h"

class PassengerQueuer
{
    public:
        PassengerQueuer(std::shared_ptr<SimLogger> logger) : logger(logger), queueTime(0), 
            passengerCount(0), parsingDone(false) {};
        void beginQueue();
        std::chrono::time_point<std::chrono::high_resolution_clock> getStartTime() const;
        int getPickUpRequests();
        void removeRequests(int floorID);
        std::shared_ptr<Floor> getFloor(int floorID);
        bool noPassengersWaiting();
        bool isParseDone();
        int getPassengerCount() const;

    protected:
        void addPickUpRequest(int floorID);
        void addPassengerToFloor(std::shared_ptr<Passenger> newPassenger);

    private:
        std::shared_ptr<SimLogger> logger;                                              //variable to access logger class
        std::chrono::time_point<std::chrono::high_resolution_clock> queueStart;         //exact time when first passenger was queued
        int queueTime;                                                                  //stores time when last passenger was queued
        std::map<int, std::shared_ptr<Floor>> floorList;                                //list of all floors and passengers waiting on them
        std::list<int> requestsQueue;                                                  //queue of pick up requests 
        std::set<int> pickUpRequests;                                                   //list of floors to pick up passengers
        std::mutex floorMutex;                                                          //for thread safety, used for locks so threads don't collide
        std::mutex requestsMutex;                                                       //for thread safety, used for locks so threads don't collide
        std::mutex parseMutex;
        bool parsingDone;
        int passengerCount;
    };

#endif