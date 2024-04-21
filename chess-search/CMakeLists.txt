cmake_minimum_required(VERSION 3.26)
project(chess)

set(CMAKE_CXX_STANDARD 17)

set(SFML_DIR "C:/Libraries/SFML-2.5.1/lib/cmake/SFML")
find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

add_executable(chess main.cpp
)

target_link_libraries(chess sfml-system sfml-window sfml-graphics sfml-audio)
