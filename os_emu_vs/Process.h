#pragma once
#include "ICommand.h"
#include "TypedefRepo.h"
#include "MemoryManager.h"
#include <chrono>
#include <string>
#include <vector>

class Process {

public:
	/*struct RequirementFlags
	{
		bool requireFiles;
		int numFiles;
		bool requireMemory;
		int memoryRequired;
	};*/

	enum ProcessState
	{
		READY,
		RUNNING,
		WAITING,
		FINISHED
	};

	Process(int pid, String name);
	Process() = default;

	void addCommand(ICommand::CommandType commandType);
	void executeCurrentCommand() const;
	void moveToNextLine();

	bool isFinished() const;
	// int getRemainingTime() const;
	int getCommandCounter() const;
	int getLinesOfCode() const;
	int getPID() const;
	int getCPUCoreID() const;
	ProcessState getState() const;
	String getName() const;
	std::chrono::time_point<std::chrono::system_clock> getCreationTime() const;

	// setters
	void setState(ProcessState state);
	void setCpuCoreId(int _cpuCoreId);
	bool allocateMemory();
	void deallocateMemory();
	bool hasMemory() const;

private:
	int pid;
	String name;
	typedef std::vector<std::shared_ptr<ICommand>> CommandList;
	CommandList commandList;

	int commandCounter;
	int cpuCoreId = -1;
	ProcessState currentState;
	std::chrono::time_point<std::chrono::system_clock> creationTime;

	// RequirementFlags requirements;
	// friend class ResourceEmulator
	friend class FCFSScheduler;
	bool hasMemoryAllocated = false;
};