#include "Elevator.h"
#include <thread>
#include <chrono>

using namespace std;

/**
 * @brief Based on the amount of space available, add passengers 
 *        inside the elevator based on the number of people
 *        waiting
 * 
 * @param passengers (who were added to the elevator) 
 */
void Elevator::pickUpPassengers(shared_ptr<Floor> floorObject)
{   
    lock_guard<mutex> lock(elevatorMutex);

    if(floorObject == nullptr)
    {   return; }

    //verify that the elevator is not at max 
    //and greater than or equal to 0
    if (passengersInElevator.size() < MAX_CAPACITY && passengersInElevator.size() >= 0)
    {      
        int queueSize = static_cast<int>(floorObject->getQueueSize()); 
        //check if there are no passengers waiting on the floor
        if(queueSize == 0)
        {   return; }

        //check how much space is available, if there is enough
        //add as much passengers as there is space by checking the
        //waiting number of Passengers on the floor
        int spaceAvailable = MAX_CAPACITY - static_cast<int>(passengersInElevator.size());
        int floorID = 0;
        lock_guard<mutex> lock(requestsMutex);
        if(spaceAvailable == 1 || queueSize == 1)
        {
            auto passenger = floorObject->popFromQueue();
            passengersInElevator.insert({passenger->getDesiredLocation(), passenger});
            logger->addLogMessage("Elevator " + elevatorID + " picked up 1 passenger on floor " + to_string(currentFloor));

            //store riding passengers drop off requests
            
            floorID = passenger->getDesiredLocation();
            if (dropOffRequests.find(floorID) == dropOffRequests.end() ) 
            {
                requestsQueue.push(floorID);
                dropOffRequests.insert(floorID);
            }
        }
        else
        {
            int max = (spaceAvailable < queueSize) ? spaceAvailable : queueSize;
            for(int i = 0; i < max; i++)
            {
                auto passenger = floorObject->popFromQueue();
                floorID = passenger->getDesiredLocation();
                passengersInElevator.insert({floorID, passenger});

                //store riding passengers drop off requests
                if (dropOffRequests.find(floorID) == dropOffRequests.end() ) 
                {
                    requestsQueue.push(floorID);
                    dropOffRequests.insert(floorID);
                }
            }
            logger->addLogMessage("Elevator " + elevatorID + " picked up " + to_string(max) + " passengers on floor " + to_string(currentFloor));
        }
    }
    else
    {
        logger->addLogMessage("Elevator " + elevatorID + " picked up 0 passengers on floor " + to_string(currentFloor));
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
    lock_guard<mutex> lock(elevatorMutex);

    //check if there are people in the elevator
    if(passengersInElevator.size() == 0)
    {  return; }

    //erase passangers in elevator that need to leave
    //from the internal variable
    int previousSize = static_cast<int>(passengersInElevator.size());
    passengersInElevator.erase(currentFloor);
    logger->addLogMessage("Elevator " + elevatorID + " dropped off " + to_string(previousSize - passengersInElevator.size()) + 
                            " passenger(s) on floor " + to_string(currentFloor));
}

/**
 @brief Check if there are passengers waiting on current floor
 *         and check if there are passengers who desire to get of the current floor
 * 
 * @param floorObject 
 * @return true 
 * @return false 
 */
bool Elevator::shouldStop(shared_ptr<Floor> floorObject)
{
    lock_guard<mutex> lock(elevatorMutex);

    if(!(passengersInElevator.find(currentFloor) == passengersInElevator.end()))
    {   return true;    }

    if(floorObject == nullptr)
    {   return false;   }

    return (floorObject->getQueueSize() > 0);
}

/**
 * @brief Control the movement of the elevator movement.
 *        Update the internal vectors based on the current 
 *        elevator status
 * 
 * @param nextFloor (which passengers need to be dropped of or picked up at)
 */
void Elevator::moveFloors(int nextFloor, shared_ptr<PassengerQueuer> queuer)
{
    while(nextFloor != currentFloor)
    {    
        switch(elevatorStatus)
        {
            case Movement::STOPPING:
                // wait 2 seconds, since elevator takes 2 seconds to stop
                //and update the elevator status
                this_thread::sleep_for(chrono::seconds(2)); 
                elevatorStatus = Movement::STOPPED;
                break;

            case Movement::STOPPED:
                logger->addLogMessage("Elevator " + elevatorID + " stopped on floor " + to_string(currentFloor));

                //when stopped, drop off and pick up passengers
                dropOffPassengers();
                pickUpPassengers(queuer->getFloor(currentFloor));

                //Prior to moving, if at max/max floor change direction
                //Else, if not at max  or min floor and elevator  
                //continue  with the same direction
                if(currentFloor == MAX_FLOORS)
                {   elevatorDirection = Movement::DOWN;    }
                else if(currentFloor == 1)
                {   elevatorDirection = Movement::UP;    }

                //Update the elevator status to same as direction
                elevatorStatus = elevatorDirection;
                break;

            case Movement::UP:
                
                // Prior to moving up, check if there are passengers that 
                // need to be dropped off or picked up; or if requested floor is
                // is reached. Stop elevator if true, else move floors
                if(currentFloor == nextFloor || shouldStop(queuer->getFloor(currentFloor)))
                {   elevatorStatus = Movement::STOPPING;    }
                else
                {
                    // wait 10 seconds, mimic time elevator takes to move
                    this_thread::sleep_for(chrono::seconds(10)); 
                    
                    //Update floor level only when less then max
                    //else change direction elevator is moving
                    if(currentFloor < MAX_FLOORS)
                    {   
                        logger->addLogMessage("Elevator " + elevatorID + " leaving floor " + to_string(currentFloor));
                        currentFloor++;     
                    } 
                    else
                    {   
                        elevatorStatus = Movement::DOWN; 
                        elevatorDirection = Movement::DOWN;
                    }                     
                }
                break;

            case Movement::DOWN:
                // Prior to moving down, check if there are passengers that 
                // need to be dropped off or picked up; or if requested floor is
                // is reached. Stop elevator if true, else move floors
                if(currentFloor == nextFloor || shouldStop(queuer->getFloor(currentFloor)))
                {   elevatorStatus = Movement::STOPPING;    }
                else
                {                    
                    // wait 10 seconds, delay time for elevator to move
                    this_thread::sleep_for(chrono::seconds(10));      
                    
                    //Decrement the floor only when greater then min
                    //else change direction elevator is moving
                    if(currentFloor > 1)
                    {   
                        logger->addLogMessage("Elevator " + elevatorID + " leaving floor " + to_string(currentFloor));
                        currentFloor--;     
                    }               
                    else
                    {   
                        elevatorDirection = Movement::UP; 
                        elevatorStatus = Movement::UP;
                    }  
                }                
                break;
        }
    }
}

/**
 * @brief Return the floor level at the front of the queue.
 *        Remove floor from internal variables that are keeping track
 *        of requested floors
 * 
 * @return int 
 */
int Elevator::getDropOffRequests()
{   
    lock_guard<mutex> lock(requestsMutex);
    int nextFloor = (requestsQueue.empty()) ? 0 : requestsQueue.front();
    if (nextFloor != 0)
    {   
        requestsQueue.pop();
        dropOffRequests.erase(nextFloor);
    }

    return nextFloor;
}

bool Elevator::didLoopEnd()
{
    lock_guard<mutex> lock(simulationMutex);
    return simulationEnd;
}


void Elevator::simulationLoop(shared_ptr<PassengerQueuer> queuer)
{
    lock_guard<mutex> lock(simulationMutex);

    while(!simulationEnd)
    {
        int nextFloor = queuer->getPickUpRequests();
        //check for and move to requested floor
        //if valid floor level else wait once 1s before returning to loop
        if(nextFloor == 0)
        {   this_thread::sleep_for(chrono::seconds(1)); }   //1 second time delay
        else
        {   
            //go in the direction of the next queued floor for pick up
            if(nextFloor > currentFloor)
            {   
                elevatorDirection = Movement::UP;   
                elevatorStatus = Movement::UP;
            }
            else
            {   
                elevatorDirection = Movement::DOWN; 
                elevatorStatus = Movement::DOWN;
            }

            //move to queued floor for pick up
            moveFloors(nextFloor, queuer);  

            //when queued floor is reached, head to riding passengers' desired floor
            //until the queue is empty
            while(!requestsQueue.empty())
            {
                nextFloor = getDropOffRequests();
                if(nextFloor == 0)
                {   this_thread::sleep_for(chrono::seconds(1)); }   //1 second time delay
                else
                {   moveFloors(nextFloor, queuer);  }
            }
        }

        lock_guard<mutex> lock2(elevatorMutex);
        if(passengersInElevator.size() == 0 && queuer->noPassengersWaiting() && queuer->isParseDone())
        { simulationEnd =  true;    }
    }
}