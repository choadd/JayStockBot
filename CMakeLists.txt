# CMake 최소 요구 버전 설정
cmake_minimum_required(VERSION 3.16)

# 프로젝트 이름 및 버전 설정
project(stock_auto_trading VERSION 0.1)

# C++ 표준 설정
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# include 디렉토리 추가
include_directories(include)

# src 디렉토리의 모든 소스 파일 지정
file(GLOB_RECURSE SOURCES 
    "src/**/*.cpp"
)

# Executable 타겟 생성
add_executable(stock_trading ${SOURCES})

# 필요한 경우 다른 라이브러리 링크 설정
# target_link_libraries(stock_trading <필요한 라이브러리>)

# pthread 라이브러리 링크
find_package(Threads REQUIRED)
target_link_libraries(stock_trading PRIVATE Threads::Threads)