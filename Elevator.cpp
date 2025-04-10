#include "Elevator.h"
#include <thread>
#include <chrono>

using namespace std;

/**
 * @brief Construct an Elevator object which keeps track of
 *        first elevator. Initialize the floor to first, passenger
 *        amount to 0, and status to stopped.
 */
Elevator::Elevator()
{
    elevatorStatus = Movement::STOPPED;
    elevatorDirection = Movement::UP;
    currentFloor = 1;
    numberOfPassengers = 0;
}

/**
 * @brief Based on the amount of space available, add more 
 *        inside the elevator based on the number of people
 *        waiting
 * 
 * @param passengers (who were added to the elevator) 
 */
int Elevator::pickUpPassengers(int waitingPassengers)
{   
    //verify that the elevator is not at max
    if (numberOfPassengers < MAX_CAPACITY)
    {
        //check how much space is available, if there is enough
        //space for all waiting passengers add all of them in the elevator
        //else add only passangers who can fit.
        int spaceAvailable = MAX_CAPACITY - numberOfPassengers;
        if(spaceAvailable >= waitingPassengers)
        {
            numberOfPassengers += waitingPassengers;
            return waitingPassengers;
        }
        else
        {
            numberOfPassengers += spaceAvailable;
            return spaceAvailable;
        }
    }   
}

/**
 * @brief Update private variable to parallel the amount
 *        of passengers who are leaving
 * 
 * @param leavingPassengers 
 */
void Elevator::dropOffPassengers(int leavingPassengers)
{   numberOfPassengers -= leavingPassengers;   }

/**
 * @brief Control the movement of the elevator movement.
 *        Update the internal vectors based on the current 
 *        elevator status
 * 
 * @param nextFloor (which passengers need to be dropped of or picked up at)
 */
void Elevator::moveFloors(int nextFloor)
{
    switch(elevatorStatus)
    {
        case Movement::STOPPING:
            // wait 2 seconds, since elevator takes 2 seconds to stop
            this_thread::sleep_for(chrono::seconds(2)); 
            // update elevator status
            elevatorStatus = Movement::STOPPED;
            break;

        case Movement::STOPPED:
            //Prior to moving, if at max/max floor change direction
            //Else, if not at max  or min floor and elevator  
            //continue  with the same direction
            if(currentFloor = MAX_FLOORS && elevatorDirection == Movement::UP)
            {   elevatorDirection = Movement::DOWN;    }
            else if(currentFloor = 1 && elevatorDirection == Movement::DOWN)
            {   elevatorDirection = Movement::UP;    }

            //Set the status of the elevator to the same movement of the direction
            elevatorStatus = elevatorDirection;
            break;

        case Movement::UP:
            //Increment the floor only when less then max
            if(currentFloor < MAX_FLOORS)
            {   currentFloor++;     }                  // update floor level
            // wait 10 seconds, delay time for elevator to move
            this_thread::sleep_for(chrono::seconds(10)); 
            // update elevator status, if next floor with waiting 
            // passengers is reached else remain
            if(currentFloor == nextFloor)
            {   elevatorStatus = Movement::STOPPING;    }  
            break;

        case Movement::DOWN:
            //Decrement the floor only when greater then min
            if(currentFloor > 1)
            {   currentFloor--;     }               // update floor level
            // wait 10 seconds, delay time for elevator to move
            this_thread::sleep_for(chrono::seconds(10)); 
            // update elevator status, if next floor with waiting 
            // passengers is reached else remain
            if(currentFloor == nextFloor)
            {   elevatorStatus = Movement::STOPPING;    }  
            break;
    }
}

/**
 * @brief Return the current elevator's status
 * 
 * @return Movement (elevator's status)
 */
Movement Elevator::showStatus() const
{   return elevatorStatus; }

/**
 * @brief Return the current floor elevator is at
 * 
 * @return int 
 */
int Elevator::getCurrentFloor() const
{   return currentFloor; }