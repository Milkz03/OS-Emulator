#include "FCFSScheduler.h"
#include "GlobalScheduler.h"
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <thread>

#include "BaseScreen.h"
#include "ConsoleManager.h"
#include "GlobalConfig.h"
#include "os_emu_vs.h"
#include "RRScheduler.h"


GlobalScheduler* GlobalScheduler::sharedInstance = nullptr;
GlobalScheduler::GlobalScheduler(SchedulingAlgorithm algo) {

	if (algo == SchedulingAlgorithm::FCFS) {
		scheduler = std::make_shared<FCFSScheduler>();
	}
	else if (algo == SchedulingAlgorithm::ROUND_ROBIN) {
		scheduler = std::make_shared<RRScheduler>();
	}

	this->start();
	scheduler->start();
}

void GlobalScheduler::initialize()
{
	SchedulingAlgorithm algo = GlobalConfig::getInstance()->getScheduler();

	/*if (algo == SchedulingAlgorithm::ROUND_ROBIN)
	{
		std::cout << "Round Robin Scheduler\n";
	}
	else if (algo == SchedulingAlgorithm::FCFS)
	{
		std::cout << "FCFS Scheduler\n";
	}*/


	if (sharedInstance == nullptr) {
		sharedInstance = new GlobalScheduler(algo);
	}
}

GlobalScheduler* GlobalScheduler::getInstance() {
	return sharedInstance;
}

void GlobalScheduler::destroy() {
	delete sharedInstance;
	sharedInstance = nullptr;
}

GlobalScheduler::~GlobalScheduler()
{
	stopSchedulerTest();
}

void GlobalScheduler::startSchedulerTest()
{
	schedulerTestRunning = true;
	processGeneratorThread = std::thread(&GlobalScheduler::generateProcesses, this);
}

void GlobalScheduler::stopSchedulerTest()
{
	schedulerTestRunning = false;
	if (processGeneratorThread.joinable()) {
		processGeneratorThread.join();
	}

	// Add memory cleanup for all processes
	for (const auto& [name, process] : processes) {
		if (process->hasMemory()) {
			process->deallocateMemory();
		}
	}
}

void GlobalScheduler::generateProcesses()
{
	bool processCreatedInCurrentCycle = false;

	int batchProcessFrequency = GlobalConfig::getInstance()->getBatchProcessFreq();

	while (schedulerTestRunning)
	{

		if (cpuCycles % (batchProcessFrequency + 1) == 0 && !processCreatedInCurrentCycle)
		{
			const auto newProcess = createProcess("process_", Mode::KERNEL);
			const auto newBaseScreen = std::make_shared<BaseScreen>(newProcess, newProcess->getName());

			processCreatedInCurrentCycle = true;

			try {
				ConsoleManager::getInstance()->registerScreen(newBaseScreen);
			}
			catch (const std::exception& e) {
				return;
			}
		}
		else if (cpuCycles % (batchProcessFrequency + 1) != 0)
		{
			processCreatedInCurrentCycle = false;
		}
	}
}


void GlobalScheduler::tick() const {
	scheduler->execute();
}

std::shared_ptr<Process> GlobalScheduler::createProcess(std::string processName, Mode mode) {
	if (processes.find(processName) != processes.end()) {
		return nullptr;
	}

	static int nextPid = 1;
	String newProcessName;
	if (mode == Mode::KERNEL)
	{
		newProcessName = processName + std::to_string(nextPid);
	}
	else if (mode == Mode::USER) {
		newProcessName = processName;
	}

	std::shared_ptr<Process> newProcess = std::make_shared<Process>(nextPid++, newProcessName);

	newProcess->addCommand(ICommand::PRINT);

	processes[newProcessName] = newProcess;

	if (scheduler) {
		scheduler->addProcess(newProcess);
	}

	return newProcess;
}

std::shared_ptr<Process> GlobalScheduler::findProcess(String& name) const {
	auto it = processes.find(name);
	return (it != processes.end()) ? it->second : nullptr;
}

// from video
void GlobalScheduler::run() {
	this->scheduler->execute();
}

void GlobalScheduler::monitorProcesses() const {
	std::ostringstream oss;
	oss << "CPU utilization: " << scheduler->getCpuUtilization() << "%\n";
	oss << "Cores used: " << scheduler->getActiveWorkersCount() << "\n";
	oss << "Cores available: " << scheduler->workersCount - scheduler->getActiveWorkersCount() << "\n\n";

	oss << "---------------------------------------\n";
	oss << "Running processes:\n";

	for (const auto& [name, process] : processes) {
		if (process->getState() != Process::FINISHED) {
			oss << name << "\t" << formatTimestamp(process->getCreationTime()) << "\t\tCore: " << process->getCPUCoreID() << (process->getCPUCoreID() == -1 ? " " : " \t") << process->getCommandCounter() << " / " << process->getLinesOfCode() << "\n";
		}
	}
	oss << "\nFinished processes:\n";
	for (const auto& [name, process] : processes) {
		if (process->getState() == Process::FINISHED) {
			oss << name << "\t" << formatTimestamp(process->getCreationTime()) << "\t\tFinished\t" << process->getLinesOfCode() << " / " << process->getLinesOfCode() << "\n";
		}
	}
	oss << "---------------------------------------\n";

	oss << "Memory Status:\n";
	oss << "Memory Usage: " << (MemoryManager::getInstance()->getTotalMemory() -
		MemoryManager::getInstance()->getFreeMemory()) / 1024 << "KB / "
		<< MemoryManager::getInstance()->getTotalMemory() / 1024 << "KB\n";
	oss << "External Fragmentation: " << MemoryManager::getInstance()->getExternalFragmentation() / 1024 << "KB\n";
	oss << "Processes in Memory: " << MemoryManager::getInstance()->getProcessCount() << "\n\n";

	lastMonitorOutput = oss.str();
	std::cout << lastMonitorOutput;
}

void GlobalScheduler::logToFile() const {
	std::ofstream logFile("report.txt", std::ofstream::trunc);
	if (logFile.is_open()) {
		logFile << lastMonitorOutput;
	}
	logFile.close();
	std::filesystem::path filePath = std::filesystem::absolute("report.txt");
	std::cout << "Report generated at " << filePath.string() << "!\n";
}

std::string formatTimestamp(const std::chrono::time_point<std::chrono::system_clock>& timePoint) {
	std::time_t timeT = std::chrono::system_clock::to_time_t(timePoint);
	std::tm tm;
	localtime_s(&tm, &timeT);
	std::ostringstream oss;
	oss << std::put_time(&tm, "(%m/%d/%Y %I:%M:%S%p)");
	return oss.str();
}