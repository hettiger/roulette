cmake_minimum_required(VERSION 3.11)
project(Roulette)

set(CMAKE_CXX_STANDARD 11)

add_executable(Roulette main.cpp Round.cpp Round.h Bankroll.cpp Bankroll.h)