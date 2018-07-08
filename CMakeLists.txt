cmake_minimum_required(VERSION 3.11)
project(Roulette)

set(CMAKE_CXX_STANDARD 11)

add_executable(Roulette main.cpp src/Round.cpp src/Round.h src/Bankroll.cpp src/Bankroll.h src/Formatting.cpp src/Formatting.h src/PunctFacet.cpp src/PunctFacet.h src/Samplesize.cpp src/Samplesize.h src/Configuration.cpp src/Configuration.h src/Sample.cpp src/Sample.h)

target_link_libraries(Roulette pthread)
