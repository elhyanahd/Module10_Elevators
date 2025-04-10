#ifndef ELEVATOR_H
#define ELEVATOR_H

#define MAX_CAPACITY 8
#define MAX_FLOORS   100

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
        Elevator();
        int pickUpPassengers(int waitingPassengers);
        void dropOffPassengers(int leavingPassengers);
        void moveFloors(int nextFloor);
        Movement showStatus() const;
        int getCurrentFloor() const;

    private:
        int numberOfPassengers;
        int currentFloor;
        Movement elevatorDirection;
        Movement elevatorStatus;
};

#endif