// MyWorker.h
#ifndef MYWORKER_H
#define MYWORKER_H

#include "base/thread/Thread.h"

class MyWorker : public Thread {
public:
    MyWorker(int id); // 생성자
    void mainLoop() override; // mainLoop 메서드 오버라이드

private:
    int mId; // 고유한 ID
};

#endif // MYWORKER_H
