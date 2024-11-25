#pragma once
#include "IMemoryAllocator.h"
#include "Process.h"

class PagingAllocator : public IMemoryAllocator {
public:
	PagingAllocator(size_t maxMemorySize);

	void* allocate(size_t size, int processId) override;
	void deallocate(void* ptr, size_t size) override;
	std::string visualizeMemory() override;

	// other methods
	size_t getExternalFragmentation() const override;

	void* allocate(Process* process);
	void deallocate(Process* process);

private:
	size_t maximumSize;			// maxMemorySize
	size_t allocatedSize;

	std::vector<char> memory;
	//std::unordered_map<size_t, bool> allocationMap;
	std::unordered_map<size_t, int> processMap;

	size_t numFrames;
	std::unordered_map<size_t, size_t> frameMap;
	std::vector<size_t> freeFrameList;

	// new vars and methods
	size_t allocateFrames(size_t numFrames, size_t processId);
	void deallocateFrames(size_t numFrames, size_t frameIndex);
};
