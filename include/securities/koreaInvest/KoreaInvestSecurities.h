#ifndef KOREA_INVEST_SECURITIES_H
#define KOREA_INVEST_SECURITIES_H

#include <string>
#include "base/thread/Thread.h"
#include "securities/SecuritiesInterface.h" // SecuritiesInterface 포함

class KoreaInvestSecurities : public SecuritiesInterface, public Thread {
public:
    // 생성자
    KoreaInvestSecurities(const SecurityConfig& config);
    ~KoreaInvestSecurities();

    // 메서드 오버라이드
    std::string getAccessToken();
    std::string getHashkey(const std::string& data);
    bool loadConfig(const std::string& configPath);
    int getCurrentPrice(const std::string& code);

    void mainLoop();

protected:
    SecurityConfig securityConfig; // SecurityConfig 객체
    int tId;
};

#endif // KOREA_INVEST_SECURITIES_H
