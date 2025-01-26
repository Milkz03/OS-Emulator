#include "SchedulerFactory.h"
#include <iostream>

Scheduler* SchedulerFactory::createScheduler(const Config& config, ConsoleManager& consoleManager) {
    if (config.getSchedulerType() == "fcfs") {
        return new SchedulerFCFS(config.getNumCpu(), consoleManager);
    }
    else if (config.getSchedulerType() == "rr") {
        return new SchedulerRR(config.getNumCpu(), config.getQuantumCycles(), consoleManager);
    }
    else {
        std::cerr << "Unknown scheduler type in configuration." << std::endl;
        return nullptr;
    }
}