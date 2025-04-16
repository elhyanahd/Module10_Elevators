#include "Queuer.h"
#include "SimLogger.h"
#include "Elevator.h"
#include <memory>

using namespace std;

int main() 
{
    //declaration of Logger Class
    shared_ptr<SimLogger> logger = make_shared<SimLogger>();
    logger->addLogMessage("Elevator Simulation started.");
        
    //declaration to parse file and queue passangers
    shared_ptr<PassengerQueuer> queuer = make_shared<PassengerQueuer>(logger);

    //declarations for elevators
    shared_ptr<Elevator> elevatorA = make_shared<Elevator>("A", logger);
    shared_ptr<Elevator> elevatorB = make_shared<Elevator>("B", logger);
    shared_ptr<Elevator> elevatorC = make_shared<Elevator>("C", logger);
    shared_ptr<Elevator> elevatorD = make_shared<Elevator>("D", logger);

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
    logger->addLogMessage("Elevation Simulation ended.");

    auto duration = simulationEnd - queuer->getStartTime();
    auto duration_s = std::chrono::duration_cast<std::chrono::seconds>(duration);
    int time_s = duration_s.count();
    int passengerCount = queuer->getPassengerCount();
    logger->addLogMessage(to_string(passengerCount) + " were picked up and dropped off within " + to_string(time_s) + " seconds.");
    return 0;
}