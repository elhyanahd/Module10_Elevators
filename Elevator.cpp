#include "Elevator.h"
#include <thread>
#include <chrono>

using namespace std;

/**
 * @brief * @brief Based on the amount of space available, add passengers 
 *        inside the elevator 
 * 
 * @param queuer 
 * @param requestedFloor 
 */
void Elevator::pickUpPassengers(shared_ptr<PassengerQueuer> queuer, int requestedFloor)
{     
    shared_ptr<Floor> floorObject = queuer->getFloor(currentFloor);

    lock_guard<mutex> lock(elevatorMutex);
    //if floor is not in floor list of Queuer class return
    //TO DO: Try with this removed
    if(floorObject == nullptr)
    {   return; }

    //verify that the elevator is not at max capacity
    if (passengersInElevator.size() < MAX_CAPACITY)
    {    
        //remove floor from request list if it was not
        //part of the original request
        if(requestedFloor != currentFloor)
        {   queuer->removeRequests(currentFloor);   }
        //if there are no passengers waiting on the floor return  
        int queueSize = static_cast<int>(floorObject->getQueueSize()); 
        
        if(queueSize == 0)
        {   return; }

        //check how much space is available in elevator
        int spaceAvailable = MAX_CAPACITY - static_cast<int>(passengersInElevator.size());
        int floorID = 0;
        lock_guard<mutex> lock(requestsMutex);

        //Add passengers according to how much space is 
        //available in the elevator
        int max = (spaceAvailable < queueSize) ? spaceAvailable : queueSize;
        int pickedUp = 0;
        for(int i = 0; i < max; i++)
        {
            //update the floor's queue to remove the passengers
            //that enetered the elevator
            auto passenger = floorObject->popFromQueue();

            //Edge case for if elevators, stop at the same floor
            if (passenger == nullptr)
            {   continue;   }

            floorID = passenger->getDesiredLocation();
            passenger->setElevatorInfo(chrono::high_resolution_clock::now());          //record when passenger began riding elevator 
            passengersInElevator.insert({floorID, passenger});
            pickedUp++;

            //store riding passengers' drop off requests
            if (dropOffRequests.find(floorID) == dropOffRequests.end() ) 
            {
                requestsQueue.push_back(floorID);
                dropOffRequests.insert(floorID);
            }
        }
        //Update the logger with information on the number of passengers that were picked up
        logger->addLogMessage("Elevator " + elevatorID + " picked up " + to_string(pickedUp) + 
                                " passenger(s) on floor " + to_string(currentFloor));
    }  
}

/**
 * @brief Drop of passengers that are inside the elevator.
 *        Update private variable to parallel the amount
 *        of passengers who are leaving
 */
void Elevator::dropOffPassengers()
{  
    lock_guard<mutex> lock(elevatorMutex);
    lock_guard<mutex> lock2(requestsMutex);

    // if there are no passengers in the elevator
    //and there are no passengers with the current 
    //floor's drop of request return 
    if(passengersInElevator.size() == 0 || passengersInElevator.find(currentFloor) == passengersInElevator.end())
    {  return; }

    //erase passangers in elevator that need to leave
    //from the internal variable
    int amount = 0;
    vector<shared_ptr<Passenger>> forAverage;
    for (auto it = passengersInElevator.lower_bound(currentFloor); it != passengersInElevator.upper_bound(currentFloor); )
    {
        it->second->setReachedInfo(std::chrono::high_resolution_clock::now());      //record when passenger exited
        forAverage.push_back(it->second);                                           //store object for average calculation
        // Erase and advance safely
        it = passengersInElevator.erase(it);
        ++amount;
    }
    logger->addLogMessage("Elevator " + elevatorID + " dropped off " + to_string(amount) + 
                            " passenger(s) on floor " + to_string(currentFloor));

    //adding previously obtained objects to list
    //did outside of loop to avoid halting other threads
    {
        lock_guard<mutex> lock(*averageTimeMutex);
        averageTime->insert(averageTime->end(), forAverage.begin(), forAverage.end());
    }

    //if extra dropped offs were made, remove floor from requests
    if (dropOffRequests.find(currentFloor) != dropOffRequests.end() ) 
    {
        requestsQueue.remove(currentFloor);
        dropOffRequests.erase(currentFloor);
    }
}

/**
 @brief Determine if the elevaor should stop, by checking:
 *      1. for passengers who need to get off
 *      2. if elevator is at max capacity
 *      3. for passengers waiting on current floor,
 * 
 * @param floorObject 
 * @return true 
 * @return false 
 */
bool Elevator::shouldStop(shared_ptr<Floor> floorObject)
{
    lock_guard<mutex> lock(elevatorMutex);
    bool stop = false;

    if(!(passengersInElevator.find(currentFloor) == passengersInElevator.end()))
    {   stop = true;    }
    else if(floorObject == nullptr || passengersInElevator.size() == MAX_CAPACITY)
    {   stop = false;   }
    else
    { stop = (floorObject->getQueueSize() > 0);  }

    return stop;
}

/**
 * @brief Control the movement of the elevator movement.
 *        Update the internal variables based on the current 
 *        elevator status. Stop movement loop only when requested 
 *        floor has been reached.
 * 
 * @param nextFloor (current floor request)
 * @param queuer 
 */
void Elevator::moveFloors(int nextFloor, shared_ptr<PassengerQueuer> queuer)
{
    bool stopMoving = false;
    int floorReached = 0;
    
    while(!stopMoving)
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
                pickUpPassengers(queuer, nextFloor);
                logger->addLogMessage("Elevator " + elevatorID + " now has " + to_string(passengersInElevator.size()) + " passenger(s)");

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
                if( shouldStop(queuer->getFloor(currentFloor)))
                {   elevatorStatus = Movement::STOPPING;    }
                else
                {
                    // wait 10 seconds, mimic time elevator takes to move
                    this_thread::sleep_for(chrono::seconds(5)); 
                    
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
                if( shouldStop(queuer->getFloor(currentFloor)))
                {   elevatorStatus = Movement::STOPPING;    }
                else
                {                    
                    // wait 10 seconds, delay time for elevator to move
                    this_thread::sleep_for(chrono::seconds(5));      
                    
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

        //if at given floor and finished dropping/picking up passengers
        //then end loop
        floorReached = (elevatorStatus == Movement::UP) ? nextFloor + 1 : nextFloor - 1;
        if(currentFloor == floorReached && elevatorStatus == elevatorDirection)
        {   stopMoving = true;  }
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
        requestsQueue.pop_front();
        dropOffRequests.erase(nextFloor);
    }

    return nextFloor;
}

/**
 * @brief Return whether the Elevator object's
 *        finished picking up and dropping off passengers
 * 
 * @return true 
 * @return false 
 */
bool Elevator::didLoopEnd()
{
    lock_guard<mutex> lock(simulationMutex);
    return simulationEnd;
}

/**
 * @brief When called, sets the elvator direction to the 
 *        based on the given requested floor 
 * 
 * @param nextFloor 
 */
void Elevator::setDirection(int nextFloor)
{
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
}

/**
 * @brief Loops through until CSV file is parsed, all passengers
 *        in the elevator are dropped off, and there are no passengers 
 *        waiting to be picked up on the floor
 * 
 *       While elevator simulation has not ended, check for pick up 
 *       requests (retrived from the parsed file) and drop off requests
 *       (retrieved from pciked up passengers). Move elevator to satisfy
 *       the requests it receives.
 * 
 * @param queuer 
 */
void Elevator::simulationLoop(shared_ptr<PassengerQueuer> queuer)
{
    lock_guard<mutex> lock(simulationMutex);

    while(!simulationEnd)
    {
        //check for pick up requests if there are passengers waiting
        //and if elevator is not at max capacity
        int nextFloor = (passengersInElevator.size() == MAX_CAPACITY || (queuer->noPassengersWaiting() && queuer->isParseDone())) 
            ? 0 : queuer->getPickUpRequests();
        if(nextFloor != 0)
        {   
            setDirection(nextFloor);
            moveFloors(nextFloor, queuer);  
        }

        //once pick up is complete, check for drop off requests
        //perform those drop offs until elevator is empty 
        //(aka requests are no longer available)
        bool needToDropOff = true;
        while(needToDropOff)
        {
            nextFloor = getDropOffRequests();
            if(nextFloor != 0)
            {   
                setDirection(nextFloor);
                moveFloors(nextFloor, queuer);  
            }
            else
            {   needToDropOff = false;  }
        }

        this_thread::sleep_for(chrono::seconds(1));   //1 second time delay

        //check if simulation should end
        if(passengersInElevator.empty() && queuer->noPassengersWaiting() && queuer->isParseDone())
        {   simulationEnd =  true;  }
    }
}