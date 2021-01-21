cmake_minimum_required(VERSION 3.17)
project(cp)

find_package(nlohmann_json 3.9.1 REQUIRED)

set(CMAKE_CXX_STANDARD 17)

add_executable(cp main.cpp errors.h environment.h environment.cpp customlinkedlist.h aeroflot.h aeroflot.cpp)
target_link_libraries( cp PRIVATE nlohmann_json::nlohmann_json )