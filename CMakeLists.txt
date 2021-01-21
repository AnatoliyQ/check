cmake_minimum_required(VERSION 3.17)
project(newtest)

find_package(nlohmann_json 3.9.1 REQUIRED)

set(CMAKE_CXX_STANDARD 17)

add_executable(newtest main.cpp errors.h environment.h environment.cpp customlinkedlist.h aeroflot.h aeroflot.cpp)
target_link_libraries( newtest PRIVATE nlohmann_json::nlohmann_json )