#ifndef FLOOR_H
#define FLOOR_H
#include <list>
#include <memory>
#include "Passenger.h"

class Floor 
{
    public:
        Floor() : floorLevel(0) {};
        Floor(int level);
        Floor(int level, std::list<std::shared_ptr<Passenger>> newQueue);
        int getQueueSize() const;
        void addToQueue(std::shared_ptr<Passenger> newPerson);
        int getFloorLevel() const;
        std::list<std::shared_ptr<Passenger>> getWaitingPassengers();

    private:
        std::list<std::shared_ptr<Passenger>> passengerQueue;
        int floorLevel;
};

#endif