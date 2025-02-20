#pragma once

#include "ConsoleManager.h"
#include "Process.h"
#include "Scheduler.h"
#include "ThreadSafeQueue.h"
#include <atomic>
#include <condition_variable>
#include <map>
#include <thread>
#include <unordered_set>
#include <vector>

class SchedulerFirstComeFirstServe : public Scheduler {
public:
	SchedulerFirstComeFirstServe(int numCores, ConsoleManager& manager);
	~SchedulerFirstComeFirstServe();

	void addProcess(Process* process) override;
	int getTotalCores() const override;
	int getBusyCores() const override;

	std::map<Process*, int> getRunningProcesses() const override;
	std::vector<Process*> getQueuedProcesses() const override;
	std::vector<Process*> getFinishedProcesses() const override;

private:
	void schedulerLoop() override;
	void workerLoop(int coreId);

	int numCores;
	std::vector<std::thread> workerThreads;
	std::thread schedulerThread;

	ThreadSafeQueue<Process*> processQueue;

	std::atomic<bool> running;
	std::atomic<bool> paused;
	std::mutex pauseMutex;
	std::condition_variable pauseCV;

	struct Worker {
		int coreId = 0;
		std::atomic<bool> busy{ false };
		Process* currentProcess = nullptr;
		std::thread thread;
		std::mutex mtx;
		std::condition_variable cv;
	};

	std::vector<Worker*> workers;

	ConsoleManager& consoleManager;

	std::vector<Process*> allProcesses;
	mutable std::mutex allProcessesMutex;

	std::unordered_set<Process*> queuedProcessesSet;
	mutable std::mutex queuedProcessesMutex;

	std::atomic<unsigned int> cpuCycles;
};