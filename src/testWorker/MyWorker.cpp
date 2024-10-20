// MyWorker.cpp
#include "testWorker/MyWorker.h"
#include <iostream>
#include <thread>
#include <chrono>

MyWorker::MyWorker(int id) : mId(id) {} // 생성자에서 ID 초기화

void MyWorker::mainLoop() {
    while (isRunning()) { // isRunning()은 Thread 클래스에서 정의한 메서드
        std::cout << "MyWorker " << mId << " is running..." << std::endl; // ID 출력
        // 필요한 작업 수행
        std::this_thread::sleep_for(std::chrono::seconds(1)); // 1초 대기
    }
}
