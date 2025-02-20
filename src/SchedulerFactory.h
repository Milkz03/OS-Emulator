#pragma once
#include "Config.h"
#include "Scheduler.h"
#include "SchedulerFirstComeFirstServe.h"
#include "SchedulerRoundRobin.h"

class SchedulerFactory {
public:
	static Scheduler* createScheduler(const Config& config, ConsoleManager& consoleManager);
};

