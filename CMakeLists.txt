cmake_minimum_required(VERSION 3.10)
project(leveldb_test)

# C++ 표준 설정
set(CMAKE_CXX_STANDARD 11)

# LevelDB 서브모듈 추가
add_subdirectory(external/leveldb)

# 실행 파일 생성 및 LevelDB 라이브러리 링크
add_executable(my_app main.cpp)
target_link_libraries(my_app PRIVATE leveldb)
