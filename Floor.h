#ifndef FLOOR_H
#define FLOOR_H
#include <queue>
#include "Passenger.h"

class Floor 
{
    public:
        Floor() : floorLevel(0) {};
        Floor(int level);
        Floor(int level, std::queue<Passenger> newQueue);
        int getQueueSize() const;
        void addToQueue(Passenger newPerson);
        int getFloorLevel() const;

    private:
        std::queue<Passenger> passengerQueue;
        int floorLevel;
};

#endif