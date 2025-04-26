#ifndef ELEVATOR_H
#define ELEVATOR_H

#define MAX_CAPACITY 8
#define MAX_FLOORS   100

#include "SimLogger.h"
#include "Floor.h"
#include "Queuer.h"
#include <map>
#include <set>
#include <memory>
#include <mutex>
// Movement status of the elevator
enum class Movement 
{
    STOPPED, 
    STOPPING, 
    UP, 
    DOWN
};

class Elevator
{
    public:
        explicit Elevator::Elevator(std::string ID, std::shared_ptr<SimLogger> logger, 
            std::shared_ptr<std::list<std::shared_ptr<Passenger>>> averageTime, std::shared_ptr<std::mutex> averageTimeMutex): 
            elevatorStatus(Movement::UP), elevatorDirection(Movement::UP), currentFloor(1), averageTime(averageTime),
            elevatorID(ID), logger(logger), simulationEnd(false), passengersInElevator({}), averageTimeMutex(averageTimeMutex) {}
        void simulationLoop(std::shared_ptr<PassengerQueuer> queuer);
        bool didLoopEnd();
        
    protected:
        void pickUpPassengers(std::shared_ptr<PassengerQueuer> queuer, int requestedFloor);
        void dropOffPassengers();
        bool shouldStop(std::shared_ptr<Floor> floorObject);
        void moveFloors(int nextFloor, std::shared_ptr<PassengerQueuer> queuer);
        int getDropOffRequests(); 
        void setDirection(int nextFloor);

    private:
        int currentFloor;
        bool simulationEnd;
        std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
        std::string elevatorID;
        Movement elevatorDirection;
        Movement elevatorStatus;
        std::multimap<int, std::shared_ptr<Passenger>> passengersInElevator;
        std::shared_ptr<SimLogger> logger;
        std::shared_ptr<std::list<std::shared_ptr<Passenger>>> averageTime;
        std::shared_ptr<std::mutex> averageTimeMutex;
        std::mutex elevatorMutex;
        std::mutex simulationMutex;
        std::mutex requestsMutex;
        std::list<int> requestsQueue;                                                  //queue of pick up requests 
        std::set<int> dropOffRequests;                                                   //list of floors to drop off passengers
};

#endif