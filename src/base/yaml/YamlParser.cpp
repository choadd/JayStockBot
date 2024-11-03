#include "base/yaml/YamlParser.h"
#include <yaml-cpp/yaml.h>
#include <iostream>
#include <stdexcept>

YamlParser::YamlParser(const std::string& filePath) : filePath(filePath) {}

int YamlParser::setDefaultConfig() {
    try {
        YAML::Node config = YAML::LoadFile(filePath);
        
        yamlConfig.app_key = config["APP_KEY"].as<std::string>();
        yamlConfig.app_secret = config["APP_SECRET"].as<std::string>();
        yamlConfig.cano = config["CANO"].as<std::string>();
        yamlConfig.acnt_prdt_cd = config["ACNT_PRDT_CD"].as<std::string>();
        yamlConfig.url_base = config["URL_BASE"].as<std::string>();
        yamlConfig.discord_webhook_url = config["DISCORD_WEBHOOK_URL"].as<std::string>();

    } catch (const YAML::Exception& e) {
        std::cerr << "YAML 파싱 오류: " << e.what() << std::endl;
        throw std::runtime_error("YAML 파일을 읽는 데 실패했습니다.");
    }
    return 1;
}

Config YamlParser::getDefaultConfig() {
    try {
        std::cout << "APP_KEY: " << yamlConfig.app_key << "\n";
        std::cout << "APP_SECRET: " << yamlConfig.app_secret << "\n";
        std::cout << "CANO: " << yamlConfig.cano << "\n";
        std::cout << "ACNT_PRDT_CD: " << yamlConfig.acnt_prdt_cd << "\n";
        std::cout << "URL_BASE: " << yamlConfig.url_base << "\n";
        std::cout << "DISCORD_WEBHOOK_URL: " << yamlConfig.discord_webhook_url << "\n";

    } catch (const std::exception& e) {
        std::cerr << "에러: " << e.what() << std::endl;
    }

    return yamlConfig;
}

