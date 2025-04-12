#include "Queuer.h"
#include "SimLogger.h"
#include <memory>

using namespace std;

int main() 
{
    //declaration of Logger Class
    shared_ptr<SimLogger> logger = make_shared<SimLogger>();
    logger->addLogMessage("Elevator Simulation started.");
        
    //declaration to parse file and queue passangers
    shared_ptr<PassengerQueuer> queuer = make_shared<PassengerQueuer>(logger);

    //Thread to queue passangers, without conflicting elevators
    std::thread passengerThread([&]() { queuer->beginQueue();   });

    // std::thread elevatorThread([&]() {
    //     elevator->run(queuer); // keeps polling and moving
    // });

    passengerThread.join();
    //elevatorThread.join();

    //check for waiting passengers
    //begin moving elevators
    // shared_ptr<ElevatorController> sim = make_shared<ElevatorController>(logger, queuer);
    // sim->beginLoop();
    // logger->addLogMessage("Elevation Simulation ended.");
    return 0;
}