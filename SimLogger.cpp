#include "SimLogger.h"

using namespace std;

/**
 * @brief Construct a SimLogger object which initializes the 
 *        logger information 
 */
SimLogger::SimLogger()
{
    //set text file output
    logger = spdlog::basic_logger_mt("sim_logger", "logs/elevator_sim_log.txt", true);
    //Set Logger pattern : timestep and message type
    logger->set_pattern("[%Y-%m-%d %H:%M:%S] [%^%l%$] %v");
}

/**
 * @brief Add given message to the log  
 */
void SimLogger::addLogMessage(const string message)
{
    logger->info(message);
    
}