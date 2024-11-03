#ifndef YAML_PARSER_H
#define YAML_PARSER_H

#include <string>

struct Config {
    std::string app_key;
    std::string app_secret;
    std::string cano;
    std::string acnt_prdt_cd;
    std::string url_base;
    std::string discord_webhook_url;
};

class YamlParser {
public:
    explicit YamlParser(const std::string& filePath);
    int setDefaultConfig();
    Config getDefaultConfig();

private:
    std::string filePath;
    Config yamlConfig;
};

#endif // YAML_PARSER_H
