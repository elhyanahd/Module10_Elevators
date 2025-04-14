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
    std::thread passengerThread([&]() { queuer->beginQueue();   });

    std::thread elevatorAThread([&]() {  elevatorA->simulationLoop(queuer); });

    passengerThread.join();
    elevatorAThread.join();

    // logger->addLogMessage("Elevation Simulation ended.");
    return 0;
}