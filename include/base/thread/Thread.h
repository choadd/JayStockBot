#ifndef THREAD_H
#define THREAD_H

#include <thread>
#include <atomic>

class Thread {
public:
    Thread();
    virtual ~Thread();

    void start();
    void stop();
    void join();

    bool isRunning() const;
    
protected:
    virtual void mainLoop() = 0; // 서브클래스에서 구현해야 하는 메서드

private:
    void run();

    std::thread mThread;
    std::atomic<bool> mRunning;
};

#endif // THREAD_H
