#include "testWorker/MyWorker.h"
#include "base/thread/ThreadManager.h"
#include <vector>
#include "securities/koreaInvest/KoreaInvestSecurities.h"

int main() {
    ThreadManager manager;
    std::vector<void*> workers;

    // add security logic
    SecurityConfig security;
    security.urlBase = "https://openapi.koreainvestment.com:9443";

    // KoreaInvestSecurities 객체 생성
    KoreaInvestSecurities *ko = new KoreaInvestSecurities(security);
    workers.push_back(ko);
    manager.addThread(ko);
    // end security logic
    

    manager.startAll(); // 모든 스레드 시작

    std::this_thread::sleep_for(std::chrono::seconds(1)); // 5초 대기

    manager.stopAll(); // 모든 스레드 정지
    manager.joinAll(); // 모든 스레드 종료

    // 동적 메모리 해제
    for (auto worker : workers) {
        delete worker;
    }

    return 0;
}
