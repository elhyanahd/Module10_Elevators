#ifndef PASSENGERS_H
#define PASSENGERS_H

class Passenger 
{
    public:
        //Construct an object that sets variables to 0
        Passenger() : desired(0), current(0), startTime(0), endTime(0) {};
        Passenger(int desiredFloor, int currentFloor, int start);
        int getDesiredLocation() const;
        int getCurrentLocation() const;
        //int setEndTime() const;
        
    private:
        int desired;        //represents the passengers desired floor
        int current;        //represents the passengers current floor location
        int startTime;      //passanger time or arrival
        int endTime;        //passanger reaches end floor
};

#endif