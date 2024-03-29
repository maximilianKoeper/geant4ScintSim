#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 3.16)
project(geant4ScintSim VERSION 0.1 LANGUAGES CXX)

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

#----------------------------------------------------------------------------
find_package( Boost COMPONENTS program_options REQUIRED )
include_directories( ${Boost_INCLUDE_DIR} )

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
# Setup include directory for this project
#
include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)

#----------------------------------------------------------------------------
# If you only include this third party in PRIVATE source files, you do not need to install it
# when your main project gets installed.
#include(FetchContent)

#FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.10.5/json.tar.xz)
#FetchContent_MakeAvailable(json)
set(JSON_Install OFF CACHE INTERNAL "")

add_subdirectory(./3rdparty/json)



#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(Sim sim.cc ${sources} ${headers})
target_link_libraries(Sim PRIVATE ${Geant4_LIBRARIES} ${Boost_LIBRARIES})
target_link_libraries(Sim PRIVATE nlohmann_json::nlohmann_json)
set_target_properties(Sim PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build B1. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(SIM_SCRIPTS
  init_vis.mac
  run1.mac
  vis.mac
  )

foreach(_script ${SIM_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()


#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
#install(TARGETS exampleB1 DESTINATION bin)
