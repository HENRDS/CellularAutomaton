cmake_minimum_required(VERSION 3.10)
project(Automata)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)

include_directories(${OPENCV_INCLUDE_DIRS})
add_executable(Automata main.cc Lattice.hpp Cellular.hpp GasAutomaton.cc GasAutomaton.hpp)
target_link_libraries(${CMAKE_PROJECT_NAME} ${OpenCV_LIBS})
