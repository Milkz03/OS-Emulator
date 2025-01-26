#pragma once

#include <map>
#include <vector>
#include <thread>
#include <atomic>
#include <condition_variable>
#include "Process.h"

class Process;

class Scheduler {
public:
    virtual ~Scheduler() = default;
    virtual void addProcess(Process* process) = 0;
    virtual void start();
    virtual void stop();
    virtual void pause();
    virtual void resume();
    virtual bool isRunning() const;
    virtual bool isPaused() const;

    virtual int getTotalCores() const = 0;
    virtual int getBusyCores() const = 0;

    virtual std::map<Process*, int> getRunningProcesses() const = 0;
    virtual std::vector<Process*> getQueuedProcesses() const = 0;
    virtual std::vector<Process*> getFinishedProcesses() const = 0;

protected:
    std::atomic<bool> running{ false };
    std::atomic<bool> paused{ false };
    std::condition_variable pauseCV;
    std::thread schedulerThread;

    virtual void schedulerLoop() = 0;
};
