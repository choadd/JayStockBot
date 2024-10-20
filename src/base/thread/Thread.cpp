#include "base/thread/Thread.h"

Thread::Thread() : mRunning(false) {}

Thread::~Thread() {
    stop();
}

void Thread::start() {
    mRunning = true;
    mThread = std::thread(&Thread::run, this);
}

void Thread::stop() {
    mRunning = false;
}

void Thread::join() {
    if (mThread.joinable()) {
        mThread.join();
    }
}

void Thread::run() {
    while (mRunning) {
        mainLoop(); // 서브클래스에서 구현된 메서드 호출
    }
}

bool Thread::isRunning() const {
    return mRunning; // 현재 스레드가 실행 중인지 반환
}