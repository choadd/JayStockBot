#ifndef THREADMANAGER_H
#define THREADMANAGER_H

#include <vector>
#include "Thread.h"

class ThreadManager {
public:
    void addThread(Thread* thread);
    void startAll();
    void stopAll();
    void joinAll();

private:
    std::vector<Thread*> mThreads; // 관리하는 스레드 목록
};

#endif // THREADMANAGER_H
