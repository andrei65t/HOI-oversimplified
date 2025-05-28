cmake_minimum_required(VERSION 3.10)
project(HOI)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

include_directories(headers)

add_executable(hoi
    src/main.cpp
    src/init.cpp
    src/interface.cpp
    src/countries.cpp
    src/button.cpp
)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
target_link_libraries(hoi sfml-graphics sfml-window sfml-system)
