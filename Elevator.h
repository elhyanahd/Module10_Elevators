#ifndef ELEVATOR_H
#define ELEVATOR_H

#define MAX_CAPACITY 8
#define MAX_FLOORS   100

#include "SimLogger.h"
#include "Floor.h"
#include <map>
#include <memory>
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
        Elevator(std::string ID, std::shared_ptr<SimLogger> logger);
        void pickUpPassengers(std::map<int, std::shared_ptr<Floor>> floorList);
        void dropOffPassengers();
        bool shouldStop(std::map<int, std::shared_ptr<Floor>> floorList);
        void moveFloors(int nextFloor, std::map<int, std::shared_ptr<Floor>> floorList);
        Movement showStatus() const;
        int getCurrentFloor() const;

    private:
        int currentFloor;
        std::string elevatorID;
        Movement elevatorDirection;
        Movement elevatorStatus;
        std::multimap<int, std::shared_ptr<Passenger>> passengersInElevator;
        std::shared_ptr<SimLogger> logger;
};

#endif