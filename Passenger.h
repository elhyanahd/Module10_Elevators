#ifndef PASSENGER_H
#define PASSENGER_H

#include <map>

class Passenger 
{
    public:
        //Construct an object that sets variables to 0
        Passenger() : desired(0), current(0), startTime(0), endTime(0), inElevator(false) {};
        Passenger(int desiredFloor, int currentFloor, int start, bool inside);
        int getDesiredLocation() const;
        int getCurrentLocation() const;
        bool checkIfRiding() const;
        void setElevatorCheck(const bool riding); 
        
    private:
        int desired;        //represents the passengers desired floor
        int current;        //represents the passengers current floor location
        int startTime;      //passanger time or arrival
        int endTime;        //passanger reaches end floor
        bool inElevator;    //passenger riding elevator
};

#endif