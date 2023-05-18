cmake_minimum_required(VERSION 3.23)
project(Checkers)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc -Wall")

add_executable(CLCheckers WIN32 application.cpp defs.h resources/resources.h resources/resources.rc  appconsts.h graphic_subsystem.h clcengine/clcengine_types.h clcengine/clcengine_auxiliary.h clcengine/clcengine_inits.h clcengine/clcengine_move_searching.h clcengine/clcengine_moving.h clcengine/clcengine_sequence_searching.h clcengine/clcengine_sequence_moving.h clcengine/clcengine_analyzer.h clcengine/clcengine.h)
