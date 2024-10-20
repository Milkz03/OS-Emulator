#include "AScheduler.h"
#include "SchedulerWorker.h"
#include <iostream>

void SchedulerWorker::update(bool isRunning) {
	this->running = isRunning;
}

void SchedulerWorker::run() {
	//std::cout << "SchedulerWorker #" << this->cpuCoreId << " Waiting... " << "running : " << running << std::endl;

	while (true) {
		std::shared_ptr<Process> process;

		{
			//std::cout << "Worker " << cpuCoreId << " is waiting" << std::endl;

			std::unique_lock<std::mutex> lock(scheduler->queueMutex);

			// wait when there is no process in the queue
			scheduler->queueCV.wait(lock, [this] { return !scheduler->readyQueue.empty() || !scheduler->running; }); // not empty, or not running

			//std::cout << "Worker " << cpuCoreId << " is running" << std::endl;

			if (!scheduler->running && scheduler->readyQueue.empty()) {
				return; // Exit if the scheduler stops running and there's no work left
			}

			if (!scheduler->readyQueue.empty()) {
				//std::cout << "ready queue not empty, size: " << scheduler->readyQueue.size() << std::endl;
				process = scheduler->readyQueue.front();
				scheduler->readyQueue.pop();
			}
		}

		// Process execution
		if (process) {
			process->setState(Process::RUNNING);
			process->setCpuCoreId(cpuCoreId);
			scheduler->incrementActiveWorkers();

			SchedulingAlgorithm algo = scheduler->getSchedulingAlgo();

			if (algo == SchedulingAlgorithm::FCFS)
			{
				while (!process->isFinished() && running) {
					process->executeCurrentCommand();
					process->moveToNextLine();
					IETThread::sleep(50);                       // Simulate execution delay
				}
			}
			else if (algo == SchedulingAlgorithm::ROUND_ROBIN)
			{
				const int timeSlice = 5000;		// 5 seconds
				auto startTime = std::chrono::steady_clock::now();

				while (!process->isFinished() && running)
				{
					process->executeCurrentCommand();
					process->moveToNextLine();
					IETThread::sleep(50);                       // Simulate execution delay

					auto currentTime = std::chrono::steady_clock::now();
					auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count();

					if (elapsedTime >= timeSlice) {
						break;
					}
				}

				if (!process->isFinished())
				{
					std::unique_lock<std::mutex> lock(scheduler->queueMutex);
					scheduler->readyQueue.push(process);
				}
			}


			if (process->isFinished()) {
				process->setState(Process::FINISHED);
				scheduler->decrementActiveWorkers();
			}

			//this->update(false); // Mark the worker as free
		}
	}
}

bool SchedulerWorker::isRunning() {
	return running;
}
