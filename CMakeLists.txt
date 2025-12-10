cmake_minimum_required(VERSION 2.6...3.22.1)

project(alpideSim)

option(WITH_GEANT4_UIVIS "Build with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)

# We presume the existence of three directories
file(GLOB_RECURSE sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB_RECURSE headers ${PROJECT_SOURCE_DIR}/include/*.hh)
file(GLOB_RECURSE macros_mac RELATIVE ${PROJECT_SOURCE_DIR} macros/*.mac)
file(GLOB_RECURSE macros_sh  RELATIVE ${PROJECT_SOURCE_DIR} macros/*.sh)

# Enable macros for out-of-source build
set(macros ${macros_mac} ${macros_sh})
foreach(_file ${macros})
  configure_file(
    ${_file}
    ${PROJECT_BINARY_DIR}/${_file}
    COPYONLY
    )
endforeach()

add_executable(alpideSim main.cc ${sources} ${headers})
target_link_libraries(alpideSim ${Geant4_LIBRARIES})

install(TARGETS alpideSim DESTINATION bin)
