#include "SimLogger.h"

using namespace std;

/**
 * @brief Construct a SimLogger object which initializes the 
 *        logger information 
 */
SimLogger::SimLogger(string filename)
{
    
    // Try to retrieve existing logger
    logger = spdlog::get(filename);

    // If it doesn't exist yet, create and register it
    if (!logger)
    {
        //set text file output
        logger = spdlog::basic_logger_mt(filename, "logs/" + filename, true);
        // Set the logger to output infomation messages set immediatley when given
        logger->flush_on(spdlog::level::info);
        //Set Logger pattern : timestep and message type
        logger->set_pattern("[%Y-%m-%d %H:%M:%S] [%^%l%$] %v");
    }
}

/**
 * @brief Add given message to the log  
 */
void SimLogger::addLogMessage(const string message)
{    logger->info(message); }