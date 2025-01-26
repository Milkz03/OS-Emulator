#include "Scheduler.h"

void Scheduler::start() {
    if (running.load()) return;
    running.store(true);
    paused.store(false);
    schedulerThread = std::thread([this]() { this->schedulerLoop(); });
}

void Scheduler::stop() {
    if (!running.load()) return;
    running.store(false);
    paused.store(false);
    pauseCV.notify_all();
    if (schedulerThread.joinable()) {
        schedulerThread.join();
    }
}

void Scheduler::pause() {
    if (!running.load() || paused.load()) return;
    paused.store(true);
}

void Scheduler::resume() {
    if (!running.load() || !paused.load()) return;
    paused.store(false);
    pauseCV.notify_all();
}

bool Scheduler::isRunning() const {
    return running.load();
}

bool Scheduler::isPaused() const {
    return paused.load();
}
