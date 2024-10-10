#pragma once
#include <string>
#include <vector>
#include <chrono>
#include "TypedefRepo.h"
#include "ICommand.h"

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

    void test_generate100PrintCommands();

    // setters
    void setState(ProcessState state);

    //void test_generateRandomCommands(int limit);

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
};