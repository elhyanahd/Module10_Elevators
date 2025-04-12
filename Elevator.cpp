#include "Elevator.h"
#include <thread>
#include <chrono>

using namespace std;

/**
 * @brief Construct an Elevator object which keeps track of
 *        first elevator. Initialize the floor to first, passenger
 *        amount to 0, and status to stopped.
 */
Elevator::Elevator(string ID, shared_ptr<SimLogger> logger)
{
    elevatorStatus = Movement::STOPPED;
    elevatorDirection = Movement::UP;
    currentFloor = 1;
    logger = logger;
    elevatorID = ID;
    passengersInElevator = {};
}

/**
 * @brief Based on the amount of space available, add passengers 
 *        inside the elevator based on the number of people
 *        waiting
 * 
 * @param passengers (who were added to the elevator) 
 */
void Elevator::pickUpPassengers(map<int, shared_ptr<Floor>> floorList)
{   
    //verify that the elevator is not at max 
    //and greater than 0
    if (passengersInElevator.size() < MAX_CAPACITY && passengersInElevator.size() > 0)
    {
        //check if there are passengers waiting on the floor
        if(floorList[currentFloor]->getWaitingPassengers().size() == 0)
        {   return; }

        //check how much space is available, if there is enough
        //add as much passengers as there is space by checking the
        //waiting number of Passengers on the floor
        int spaceAvailable = MAX_CAPACITY - static_cast<int>(passengersInElevator.size());
        list<shared_ptr<Passenger>> waitingPassengers = floorList[currentFloor]->getWaitingPassengers();
        if(spaceAvailable == 1)
        {
            passengersInElevator.insert({waitingPassengers.front()->getDesiredLocation(), waitingPassengers.front()});
            waitingPassengers.pop_front();
            logger->addLogMessage("Elevator " + elevatorID + " picked up 1 passenger on floor " + to_string(currentFloor));
        }
        else
        {
            for(int i = 0; i < spaceAvailable; i++)
            {
                passengersInElevator.insert({waitingPassengers.front()->getDesiredLocation(), waitingPassengers.front()});
                waitingPassengers.pop_front();
            }
            logger->addLogMessage("Elevator " + elevatorID + " picked up " + to_string(spaceAvailable) + " passengers on floor " + to_string(currentFloor));
        }
    }   
}

/**
 * @brief Update private variable to parallel the amount
 *        of passengers who are leaving
 * 
 * @param leavingPassengers 
 */
void Elevator::dropOffPassengers()
{   
    //check if there are people in the elevator
    if(passengersInElevator.size() == 0)
    {  return; }

    //erase passangers in elevator that need to leave
    //from the internal variable
    int previousSize = static_cast<int>(passengersInElevator.size());
    passengersInElevator.erase(currentFloor);
    logger->addLogMessage("Elevator " + elevatorID + " dropped off " + to_string(previousSize - passengersInElevator.size()) + 
                            " passengers on floor " + to_string(currentFloor));
}

/**
 * @brief Check if there are passengers waiting on current floor
 *         and check if there are passengers who desire to get of the current floor
 * 
 * @return true 
 * @return false 
 */
bool Elevator::shouldStop(map<int, shared_ptr<Floor>> floorList)
{
    return (floorList[currentFloor]->getWaitingPassengers().size() > 0) || 
            (passengersInElevator.find(currentFloor) != passengersInElevator.end());
}

/**
 * @brief Control the movement of the elevator movement.
 *        Update the internal vectors based on the current 
 *        elevator status
 * 
 * @param nextFloor (which passengers need to be dropped of or picked up at)
 */
void Elevator::moveFloors(int nextFloor, map<int, shared_ptr<Floor>> floorList)
{
    while(nextFloor != currentFloor)
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
                logger->addLogMessage("Elevator" + elevatorID + " stopped on floor " + to_string(currentFloor));

                //when stopped, drop off and pick up passengers
                dropOffPassengers();
                pickUpPassengers(floorList);

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
                {   
                    logger->addLogMessage("Elevator " + elevatorID + " leaving floor " + to_string(currentFloor) + 
                                            " and heading to floor" + to_string(currentFloor++));
                    currentFloor++;     
                } 

                //change direction that elevator is moving
                //if max is reached
                if(currentFloor == MAX_FLOORS)
                {   elevatorDirection = Movement::DOWN; }  

                // wait 10 seconds, mimic time elevator takes to move
                this_thread::sleep_for(chrono::seconds(10)); 

                // update elevator status, based on passengers that need to 
                //be dropped off or picked up; or if original queued passenger
                //floor is reached
                if(currentFloor == nextFloor || shouldStop(floorList))
                {   elevatorStatus = Movement::STOPPING;    }
                break;

            case Movement::DOWN:
                //Decrement the floor only when greater then min
                if(currentFloor > 1)
                {   
                    logger->addLogMessage("Elevator " + elevatorID + " leaving floor " + to_string(currentFloor) + 
                                            " and heading to floor" + to_string(currentFloor--));
                    currentFloor--;     
                }               

                //change direction that elevator is moving
                //if min is reached
                if(currentFloor == 1)
                {   elevatorDirection = Movement::UP; }  

                // wait 10 seconds, delay time for elevator to move
                this_thread::sleep_for(chrono::seconds(10)); 

                // update elevator status, based on passengers that need to 
                //be dropped off or picked up; or if original queued passenger
                //floor is reached
                if(currentFloor == nextFloor || shouldStop(floorList))
                {   elevatorStatus = Movement::STOPPING;    }
                break;
        }
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