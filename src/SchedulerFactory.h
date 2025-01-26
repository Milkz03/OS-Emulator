#pragma once
#include "Scheduler.h"
#include "SchedulerFCFS.h"
#include "SchedulerRR.h"
#include "Config.h"

class SchedulerFactory {
public:
    static Scheduler* createScheduler(const Config& config, ConsoleManager& consoleManager);
};

