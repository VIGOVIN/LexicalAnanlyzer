cmake_minimum_required(VERSION 3.15)
project(LexicalAnalyzer)

set(CMAKE_CXX_STANDARD 11)

#add_executable(LexicalAnalyzer Test.cpp DataStructure.h DataStructure.cpp Operation.h Operation.cpp)
add_executable(LexicalAnalyzer main.cpp DataStructure.h DataStructure.cpp Operation.h Operation.cpp)
