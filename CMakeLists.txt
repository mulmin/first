cmake_minimum_required(VERSION 3.10)
project(MyLevelDBProject)

# C++ 표준 설정
set(CMAKE_CXX_STANDARD 11)

# 디버그 모드로 빌드 설정
set(CMAKE_BUILD_TYPE Debug)

# LevelDB의 헤더 경로 설정
include_directories("${CMAKE_SOURCE_DIR}/external/leveldb/include")

# main.cpp 파일을 컴파일할 실행 파일 생성
add_executable(my_program main.cpp)

# LevelDB 라이브러리 파일의 전체 경로로 직접 링크
target_link_libraries(my_program "${CMAKE_SOURCE_DIR}/external/leveldb/build/libleveldb.a")

