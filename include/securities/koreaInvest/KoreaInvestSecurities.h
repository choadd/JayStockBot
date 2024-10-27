#ifndef KOREA_INVEST_SECURITIES_H
#define KOREA_INVEST_SECURITIES_H

#include <string>
#include "securities/SecuritiesInterface.h" // SecuritiesInterface 포함

class KoreaInvestSecurities {
public:
    // 생성자
    KoreaInvestSecurities(const SecurityConfig& config);
    ~KoreaInvestSecurities();

    // 메서드 오버라이드
    std::string getAccessToken() ;
    std::string getHashkey(const std::string& data) ;
    bool loadConfig(const std::string& configPath) ;

protected:
    SecurityConfig securityConfig; // SecurityConfig 객체
};

#endif // KOREA_INVEST_SECURITIES_H
