#include <iostream>
#include <fstream>
#include <yaml-cpp/yaml.h>
#include <curl/curl.h> // libcurl 헤더
#include <string.h>
#include <json/json.h>

#include "securities/koreaInvest/KoreaInvestSecurities.h"

// 생성자 구현
KoreaInvestSecurities::KoreaInvestSecurities(const SecurityConfig& config) {
    securityConfig = config; // securityConfig 초기화
}

KoreaInvestSecurities::~KoreaInvestSecurities() {
    // 자원 해제 및 필요한 클린업 코드 추가
    // 예: 동적으로 할당된 메모리 해제 등
}

// CURL 응답을 저장하기 위한 구조체
struct MemoryStruct {
    char *memory;
    size_t size;
};

// CURL 응답을 메모리에 저장하는 콜백 함수
static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, MemoryStruct *userp) {
    size_t realsize = size * nmemb;
    userp->memory = (char *)realloc(userp->memory, userp->size + realsize + 1);
    if (userp->memory == NULL) {
        std::cerr << "Not enough memory (realloc returned NULL)\n";
        return 0; // out of memory
    }
    memcpy(&(userp->memory[userp->size]), contents, realsize);
    userp->size += realsize;
    userp->memory[userp->size] = 0; // null-terminate the string
    return realsize;
}

// config.yaml 파일에서 설정 읽기
bool KoreaInvestSecurities::loadConfig(const std::string& configPath) {
    YAML::Node config = YAML::LoadFile(configPath);
    if (config["appKey"]) {
        securityConfig.appKey = config["appKey"].as<std::string>();
    }
    if (config["appSecret"]) {
        securityConfig.appSecret = config["appSecret"].as<std::string>();
    }
    if (config["urlBase"]) {
        securityConfig.urlBase = config["urlBase"].as<std::string>();
    }
    return true;
}

// getAccessToken 메서드 구현
std::string KoreaInvestSecurities::getAccessToken() {
    // CURL 초기화
    CURL *curl;
    CURLcode res;

    MemoryStruct chunk;
    chunk.memory = (char *)malloc(1); // 초기 메모리 할당
    chunk.size = 0; // 초기 크기

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if (curl) {
        std::string url = securityConfig.urlBase + "/oauth2/tokenP";
        std::string jsonBody = R"({
            "grant_type": "client_credentials",
            "appkey": ")" + securityConfig.appKey + R"(", 
            "appsecret": ")" + securityConfig.appSecret + R"("
        })";

        // CURL 옵션 설정
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonBody.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, curl_slist_append(NULL, "Content-Type: application/json"));

        // 요청 실행
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }

        // JSON 파싱
        std::string response(chunk.memory);
        std::string accessToken = ""; // Access token 초기화
        size_t pos = response.find("\"access_token\":");
        if (pos != std::string::npos) {
            pos += 16; // "access_token": 의 길이
            size_t endPos = response.find("\"", pos);
            accessToken = response.substr(pos, endPos - pos);
            securityConfig.token = accessToken;
            std::cout << "Access Token: " << accessToken << std::endl; // 출력 추가
        }

        // 메모리 정리
        free(chunk.memory);
        curl_easy_cleanup(curl);
        curl_global_cleanup();

        return accessToken;
    }

    return "";
}

// hashkey 메서드 구현
std::string KoreaInvestSecurities::getHashkey(const std::string& data) {
    // CURL 초기화
    CURL *curl;
    CURLcode res;

    MemoryStruct chunk;
    chunk.memory = (char *)malloc(1); // 초기 메모리 할당
    chunk.size = 0; // 초기 크기

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if (curl) {
        std::string url = securityConfig.urlBase + "/uapi/hashkey";
        
        // 요청 바디 생성
        std::string jsonBody = R"({"data": ")" + data + R"("})";

        // CURL 옵션 설정
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonBody.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, curl_slist_append(NULL, "Content-Type: application/json"));
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, curl_slist_append(NULL, ("appKey: " + securityConfig.appKey).c_str()));
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, curl_slist_append(NULL, ("appSecret: " + securityConfig.appSecret).c_str()));

        // 요청 실행
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }

        // JSON 파싱
        std::string response(chunk.memory);
        std::string hashkey = ""; // Hash key 초기화
        size_t pos = response.find("\"HASH\":");
        if (pos != std::string::npos) {
            pos += 8; // "HASH": 의 길이
            size_t endPos = response.find("\"", pos);
            hashkey = response.substr(pos, endPos - pos);
        }

        // 메모리 정리
        free(chunk.memory);
        curl_easy_cleanup(curl);
        curl_global_cleanup();

        return hashkey;
    }

    return "";
}

int KoreaInvestSecurities::getCurrentPrice(const std::string& code) {
    const std::string PATH = "uapi/domestic-stock/v1/quotations/inquire-price";
    const std::string URL = securityConfig.urlBase + "/" + PATH;

    // Debug 정보 출력
    // std::cout << "SecurityConfig:" << std::endl;
    // std::cout << "  App Key: " << securityConfig.appKey << std::endl;
    // std::cout << "  App Secret: " << securityConfig.appSecret << std::endl;
    // std::cout << "  URL Base: " << securityConfig.urlBase << std::endl;
    // std::cout << "  Token: " << securityConfig.token << std::endl;

    // 요청 헤더 설정
    struct curl_slist* headers = nullptr;
    std::string token = "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzUxMiJ9.eyJzdWIiOiJ0b2tlbiIsImF1ZCI6IjY2NmM5MjFkLWU4ZGItNDg0YS04NmEyLWU5MDMwYTZmYTY4YiIsInByZHRfY2QiOiIiLCJpc3MiOiJ1bm9ndyIsImV4cCI6MTczMDM3NjY0MSwiaWF0IjoxNzMwMjkwMjQxLCJqdGkiOiJQU0hweWZBeUlTcWhKaXI4cnpxUzdNdldtaElZMWNtUjhNU2wifQ.PCRquF-Hzv0412tWC-kj6C47dWa0fmhtob6eGY6xuYOHQBiALIMaMZOn-BBn_FPFdQ1sijM2l46t921k3HJEYA";
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, ("authorization: Bearer " + token).c_str());
    headers = curl_slist_append(headers, ("appKey: " + securityConfig.appKey).c_str());
    headers = curl_slist_append(headers, ("appSecret: " + securityConfig.appSecret).c_str());
    headers = curl_slist_append(headers, "tr_id: FHKST01010100");

    // 요청 파라미터 설정
    std::string params = "fid_cond_mrkt_div_code=J&fid_input_iscd=" + code;

    // cURL 초기화
    CURL* curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Failed to initialize cURL!" << std::endl;
        return -1; // cURL 초기화 실패 시 -1 반환
    }

    CURLcode res;
    std::string response_string;

    MemoryStruct chuck;
    chuck.memory = (char *)malloc(1); // 초기 메모리 할당
    chuck.size = 0; // 초기 크기

    // URL 설정 (GET 요청)
    curl_easy_setopt(curl, CURLOPT_URL, (URL + "?" + params).c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    
    // 응답 데이터를 저장하기 위한 콜백 설정
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chuck);

    // 요청 실행
    res = curl_easy_perform(curl);


    if (res != CURLE_OK) {
        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
    }

    std::string response(chuck.memory);
    std::string stockPrice;
    size_t outputPos = response.find("\"output\"");
    if (outputPos != std::string::npos) {
        size_t stckPrprPos = response.find("\"stck_prpr\":", outputPos);
        if (stckPrprPos != std::string::npos) {
            stckPrprPos += 12;  // "stck_prpr": 의 길이
            size_t endPos = response.find("\"", stckPrprPos + 1);
            stockPrice = response.substr(stckPrprPos + 1, endPos - stckPrprPos - 1);
            std::cout << "Stock Price: " << stockPrice << std::endl; // 추출된 stck_prpr 값 출력
            return 0;
        } else {
            std::cerr << "stck_prpr key not found in output" << std::endl;
        }
    } else {
        std::cerr << "output key not found in response" << std::endl;
    }

    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);

    // 오류 시 -1 반환
    return -1;
}


void KoreaInvestSecurities::mainLoop(){
    while (isRunning()) { // isRunning()은 Thread 클래스에서 정의한 메서드
        std::cout << "KoreaInvestSecurities is running..." << std::endl; // ID 출력
        // 필요한 작업 수행
        getAccessToken();
        std::this_thread::sleep_for(std::chrono::seconds(1)); // 1초 대기

        std::string code = "000660";
        std::cout << getCurrentPrice(code) << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1)); // 1초 대기

        
    }
}