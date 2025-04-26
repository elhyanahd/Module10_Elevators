#ifndef FLOOR_H
#define FLOOR_H
#include <list>
#include <memory>
#include <mutex>
#include "Passenger.h"

class Floor 
{
    public:
        Floor() : floorLevel(0) {};
        Floor(int level);
        int getQueueSize();
        void addToQueue(std::shared_ptr<Passenger> newPerson);
        std::shared_ptr<Passenger> popFromQueue();

    private:
        std::list<std::shared_ptr<Passenger>> passengerQueue;
        int floorLevel;
        std::mutex queueMutex;
};

#endif