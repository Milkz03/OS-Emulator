#pragma once

#include <map>
#include <vector>
#include <thread>
#include <atomic>
#include <condition_variable>
#include "Process.h"
#include "ConfigUtils.h"

class Config {
public:
    static Config& getInstance();

    bool loadConfig(const std::string& filename);

    int getNumCpu() const;
    const std::string& getSchedulerType() const;
    unsigned int getQuantumCycles() const;
    unsigned int getBatchProcessFreq() const;
    unsigned int getMinIns() const;
    unsigned int getMaxIns() const;
    unsigned int getDelaysPerExec() const;
    unsigned int getMaxOverallMem() const;
    unsigned int getMemPerFrame() const;
    unsigned int getMinMemPerProc() const;
    unsigned int getMaxMemPerProc() const;

private:
    Config();
    int numCpu;
    std::string schedulerType;
    unsigned int quantumCycles;
    unsigned int batchProcessFreq;
    unsigned int minIns;
    unsigned int maxIns;
    unsigned int delaysPerExec;
    unsigned int maxOverallMem;
    unsigned int memPerFrame;
    unsigned int minMemPerProc;
    unsigned int maxMemPerProc;
};