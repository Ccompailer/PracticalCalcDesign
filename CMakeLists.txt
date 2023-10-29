cmake_minimum_required(VERSION 3.25)
project(PracticalCalcDesign)

set(CMAKE_CXX_STANDARD 23)

add_executable(PracticalCalcDesign main.cpp
        Utilities/Observer.m.cpp
        Utilities/Exception.m.cpp
        Utilities/Publisher.m.cpp
        Backend/Stack.m.cpp
        Utilities/Utilities.m.cpp)