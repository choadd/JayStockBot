#include "testWorker/MyWorker.h"
#include "base/thread/ThreadManager.h"
#include <vector>
#include "securities/koreaInvest/KoreaInvestSecurities.h"
int main() {
    ThreadManager manager;
    std::vector<MyWorker*> workers;

    // 3개의 MyWorker 객체 생성
    for (int i = 0; i < 3; ++i) {
        MyWorker* worker = new MyWorker(i + 1); // 1부터 시작하는 ID
        workers.push_back(worker);
        manager.addThread(worker); // ThreadManager에 추가
    }

    manager.startAll(); // 모든 스레드 시작

    std::this_thread::sleep_for(std::chrono::seconds(1)); // 5초 대기

    manager.stopAll(); // 모든 스레드 정지
    manager.joinAll(); // 모든 스레드 종료

    // 동적 메모리 해제
    for (auto worker : workers) {
        delete worker;
    }

    SecurityConfig security;
    security.appKey = "";
    security.appSecret = "";
    security.urlBase = "";

    // KoreaInvestSecurities 객체 생성
    KoreaInvestSecurities ko(security);

    // 액세스 토큰 요청
    ko.getAccessToken();


    return 0;
}
