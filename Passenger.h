#ifndef PASSENGER_H
#define PASSENGER_H

#include <map>
#include <chrono>

class Passenger 
{
    public:
        //Construct an object that sets variables to 0
        Passenger() : desired(0), current(0) {};
        Passenger(int desiredFloor, int currentFloor, std::chrono::time_point<std::chrono::high_resolution_clock> start);
        int getDesiredLocation() const;
        int getCurrentLocation() const;
        void setElevatorInfo(std::chrono::time_point<std::chrono::high_resolution_clock> enterTime); 
        void setReachedInfo(std::chrono::time_point<std::chrono::high_resolution_clock> exitTime);
        int getWaitTime() const;
        int getTravelTime() const;
        
    private:
        int desired;        //represents the passengers desired floor
        int current;        //represents the passengers current floor location
        std::chrono::time_point<std::chrono::high_resolution_clock> startTime;      // wait start time
        std::chrono::time_point<std::chrono::high_resolution_clock> ridingTime;     //time got on elevator
        std::chrono::time_point<std::chrono::high_resolution_clock> endTime;        //time reaches end floor
};

#endif