#include "base/thread/ThreadManager.h"

void ThreadManager::addThread(Thread* thread) {
    mThreads.push_back(thread);
}

void ThreadManager::startAll() {
    for (auto* thread : mThreads) {
        thread->start();
    }
}

void ThreadManager::stopAll() {
    for (auto* thread : mThreads) {
        thread->stop();
    }
}

void ThreadManager::joinAll() {
    for (auto* thread : mThreads) {
        thread->join();
    }
}
