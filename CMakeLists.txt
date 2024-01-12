cmake_minimum_required(VERSION 3.25)
project(PracticalCalcDesign)

set(CMAKE_CXX_STANDARD 23)

add_executable(PracticalCalcDesign main.cpp
        Utilities/Observer.m.cpp
        Utilities/Exception.h
        Utilities/Publisher.m.cpp
        Backend/Stack.m.cpp
        Utilities/Utilities.m.cpp
        Backend/Command.m.cpp
        Backend/Command.cpp
        Backend/CommandFactory.m.cpp
        Backend/CoreCommands.m.cpp
        Backend/CommandInterpreter.m.cpp
        Backend/CommandDispatcher.m.cpp
        Backend/CommandInterpreter.cpp
        Backend/CommandManager.m.cpp
        Backend/DynamicLoader.m.cpp
        Ui/UserInterface.cpp
        Backend/PosixFactory.m.cpp
        Backend/PlatformFactory.m.cpp
        Backend/PlatformFactory.cpp
        Backend/PlatformFactory.cpp
        Backend/WindowsFactory.m.cpp
        Backend/WindowsFactory.cpp
        Backend/WindowsDynamicLoader.m.cpp)