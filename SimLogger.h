#ifndef SIMLOGGER_H
#define SIMLOGGER_H
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <string>

class SimLogger 
{
    public:
        SimLogger(std::string filename);
        void addLogMessage(const std::string message);

    private:
        std::shared_ptr<spdlog::logger> logger;     //spdlog instance for logging simulation
};

#endif