#ifndef SECURITIES_INTERFACE_H
#define SECURITIES_INTERFACE_H

#include <string>

struct SecurityConfig {
    std::string appKey;
    std::string appSecret;
    std::string urlBase;
};

class SecuritiesInterface {
public:
    SecuritiesInterface(const SecurityConfig& config);
    virtual ~SecuritiesInterface(){}; // 가상 소멸자 선언

    virtual std::string getAccessToken();
    virtual std::string getHashkey(const std::string& data);
    virtual bool loadConfig(const std::string& configPath);

protected:
    SecurityConfig securityConfig;
};

#endif // SECURITIES_INTERFACE_H
