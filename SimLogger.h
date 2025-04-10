#ifndef SIMLOGGER_H
#define SIMLOGGER_H
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <string>

class SimLogger 
{
    public:
        SimLogger();
        void addLogMessage(const std::string message);

    private:
        std::shared_ptr<spdlog::logger> logger;
};

#endif