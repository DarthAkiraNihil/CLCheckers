cmake_minimum_required(VERSION 3.23)
project(Checkers)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc -Wall")

add_executable(CLCheckers WIN32 application.cpp defs.h resources/resources.h resources/resources.rc conio21/conio2.h conio21/conio.c interface.h appconsts.h clcengine/clcengine_types.h clcengine/clcengine_main.h gamemanager.h)
