#include "SchedulerFactory.h"
#include "SchedulerFirstComeFirstServe.h"
#include "SchedulerRoundRobin.h"
#include <iostream>

Scheduler* SchedulerFactory::createScheduler(const Config& config, ConsoleManager& consoleManager) {
	if (config.getSchedulerType() == "fcfs") {
		return new SchedulerFirstComeFirstServe(config.getNumCpu(), consoleManager);
	}
	else if (config.getSchedulerType() == "rr") {
		return new SchedulerRoundRobin(config.getNumCpu(), config.getQuantumCycles(), consoleManager);
	}
	else {
		std::cerr << "Unknown scheduler type in configuration." << std::endl;
		return nullptr;
	}
}