#include "Queuer.h"
#include "SimLogger.h"
#include "Elevator.h"
#include <memory>

using namespace std;

/**
 * @brief Main thread of elevator simulation
 *        Runs 5 threads: four elevator threads and 1 parser
 * 
 * @return int 
 */
int main() 
{
    //declaration of Logger Class
    shared_ptr<SimLogger> queuerLogger = make_shared<SimLogger>("QueuerLog.txt");
    shared_ptr<SimLogger> elevatorALogger = make_shared<SimLogger>("ElevatorALog.txt");
    shared_ptr<SimLogger> elevatorBLogger = make_shared<SimLogger>("ElevatorBLog.txt");
    shared_ptr<SimLogger> elevatorCLogger = make_shared<SimLogger>("ElevatorCLog.txt");
    shared_ptr<SimLogger> elevatorDLogger = make_shared<SimLogger>("ElevatorDLog.txt");
    queuerLogger->addLogMessage("Elevator Simulation started.");
    elevatorALogger->addLogMessage("Elevator Simulation started.");
    elevatorBLogger->addLogMessage("Elevator Simulation started.");
    elevatorCLogger->addLogMessage("Elevator Simulation started.");
    elevatorDLogger->addLogMessage("Elevator Simulation started.");
        
    //declaration to parse file and queue passangers
    shared_ptr<PassengerQueuer> queuer = make_shared<PassengerQueuer>(queuerLogger);

    //declaration of list that will be used to calculate average wait time and average travel time
    shared_ptr<list<shared_ptr<Passenger>>> averageTime = make_shared<list<shared_ptr<Passenger>>>();
    shared_ptr<mutex> averageTimeMutex = make_shared<mutex>();

    //declarations for elevators
    shared_ptr<Elevator> elevatorA = make_shared<Elevator>("A", elevatorALogger, averageTime, averageTimeMutex);
    shared_ptr<Elevator> elevatorB = make_shared<Elevator>("B", elevatorBLogger, averageTime, averageTimeMutex);
    shared_ptr<Elevator> elevatorC = make_shared<Elevator>("C", elevatorCLogger, averageTime, averageTimeMutex);
    shared_ptr<Elevator> elevatorD = make_shared<Elevator>("D", elevatorDLogger, averageTime, averageTimeMutex);

    //Thread to queue passangers, without conflicting elevators
    thread passengerThread([&]() { queuer->beginQueue();   });

    thread elevatorAThread([&]() {  elevatorA->simulationLoop(queuer); });
    thread elevatorBThread([&]() {  elevatorB->simulationLoop(queuer); });
    thread elevatorCThread([&]() {  elevatorC->simulationLoop(queuer); });
    thread elevatorDThread([&]() {  elevatorD->simulationLoop(queuer); });

    passengerThread.join();
    elevatorAThread.join();
    elevatorBThread.join();
    elevatorCThread.join();
    elevatorDThread.join();

    chrono::time_point<chrono::high_resolution_clock> simulationEnd = chrono::high_resolution_clock::now();
    queuerLogger->addLogMessage("Elevator Simulation ended.");
    elevatorALogger->addLogMessage("Elevator Simulation ended.");
    elevatorBLogger->addLogMessage("Elevator Simulation ended.");
    elevatorCLogger->addLogMessage("Elevator Simulation ended.");
    elevatorDLogger->addLogMessage("Elevator Simulation ended.");

    int passengerCount = queuer->getPassengerCount();
    queuerLogger->addLogMessage(to_string(passengerCount) + " were picked up and dropped off.");

    //Calculate average wait time and travel time
    double waitTime = 0;
    double travelTime = 0;
    for(auto passenger : *averageTime)
    {
        waitTime += passenger->getWaitTime();
        travelTime += passenger->getTravelTime();
    }

    waitTime = waitTime / passengerCount;
    travelTime = travelTime / passengerCount;
    queuerLogger->addLogMessage("Average wait time is: " + to_string(waitTime) + " seconds");
    queuerLogger->addLogMessage("Average travel time is: " + to_string(travelTime) + " seconds");
    return 0;
}