#ifndef SECURITIES_INTERFACE_H
#define SECURITIES_INTERFACE_H

#include <string>

struct SecurityConfig {
    std::string appKey;
    std::string appSecret;
    std::string urlBase;
    std::string token;
};

class SecuritiesInterface {
public:
    virtual std::string getAccessToken() = 0;
    virtual std::string getHashkey(const std::string& data) = 0;
    virtual bool loadConfig(const std::string& configPath) = 0;

protected:
    SecurityConfig securityConfig;
};

#endif // SECURITIES_INTERFACE_H
