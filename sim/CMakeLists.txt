cmake_minimum_required(VERSION 3.10)


# set the project name and version
project(geant4ScintSim VERSION 0.1)

# Look for Geant4 installation
find_package(Geant4 REQUIRED COMPONENTS qt ui_all vis_all)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# add executable
add_executable(Sim sim.cpp)
