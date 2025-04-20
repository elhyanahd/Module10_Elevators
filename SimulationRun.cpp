#include "Queuer.h"
#include "SimLogger.h"
#include "Elevator.h"
#include <memory>

using namespace std;

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

    //declarations for elevators
    shared_ptr<Elevator> elevatorA = make_shared<Elevator>("A", elevatorALogger);
    shared_ptr<Elevator> elevatorB = make_shared<Elevator>("B", elevatorBLogger);
    shared_ptr<Elevator> elevatorC = make_shared<Elevator>("C", elevatorCLogger);
    shared_ptr<Elevator> elevatorD = make_shared<Elevator>("D", elevatorDLogger);

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

    auto duration = simulationEnd - queuer->getStartTime();
    auto duration_s = std::chrono::duration_cast<std::chrono::seconds>(duration);
    int time_s = static_cast<int>(duration_s.count());
    int passengerCount = queuer->getPassengerCount();
    queuerLogger->addLogMessage(to_string(passengerCount) + " were picked up and dropped off within " + to_string(time_s) + " seconds.");
    return 0;
}