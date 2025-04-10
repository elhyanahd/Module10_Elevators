cmake_minimum_required(VERSION 3.5)
project(Module10Assignment)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Module10Tests Floor.h Floor.cpp Passenger.h Passenger.cpp Simulation.h Simulation.cpp)

#adding librariey for sdplog
include(FetchContent)
FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG v1.13.0 # or latest
)
FetchContent_MakeAvailable(spdlog)

target_link_libraries(Module10Tests PRIVATE spdlog::spdlog)

#Adding/Including libraries for Catch2 usage
# find_package(Catch2 REQUIRED PATHS "${CMAKE_CURRENT_SOURCE_DIR}/Catch2/include")
# target_link_libraries(Module6Tests Catch2::Catch2WithMain)

# include(CTest)
# include(Catch)
# catch_discover_tests(Module6Tests)
