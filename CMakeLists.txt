cmake_minimum_required(VERSION 2.8.11)

project(Mi4Pd)

set(PROJECT_NAME Mi4Pd)
set(CMAKE_OSX_ARCHITECTURES "i386;x86_64")

if(WIN32)
    message("windows support limited")
endif()

if (APPLE)
    set(EXECUTABLE_NAME Mi4Pd)
elseif (UNIX)
    set(EXECUTABLE_NAME mi4pd)
endif (APPLE)

set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -ffast-math -funroll-loops -fomit-frame-pointer")


message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Processor: ${CMAKE_SYSTEM_PROCESSOR}")

if (APPLE)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DTEST") 
elseif (UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC") 
    if (${CMAKE_SYSTEM_PROCESSOR} MATCHES "^arm")
        if (BELA)
            message(STATUS "BELA optimized")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=armv7-a -mtune=cortex-a8 -mfloat-abi=hard -mfpu=neon -ftree-vectorize --fast-math")
        else ()
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=armv7-a -mfpu=vfpv3 -mfloat-abi=hard")
        endif (BELA)
    else ()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DTEST") 
    endif ()
elseif(WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DTEST")
    set(PD_CMAKE_PATH C:/Program\ Files\ \(x86\)/Pd/bin)
endif (APPLE)


## setup rpath for linux and macos
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
if (NOT APPLE)
    set(CMAKE_INSTALL_RPATH "$ORIGIN/:$ORIGIN/../lib")
else()
  set(CMAKE_INSTALL_NAME_DIR "@rpath")
  set(CMAKE_INSTALL_RPATH "@executable_path/../lib;@loader_path")
  set(CMAKE_BUILD_WITH_INSTALL_NAME_DIR TRUE)
endif()



set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/release/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/release/bin)

############
add_subdirectory(mi)
add_subdirectory(brds)
add_subdirectory(clds)
add_subdirectory(clds_reverb)
add_subdirectory(grds)
add_subdirectory(lmnts)
add_subdirectory(rngs)
add_subdirectory(rngs_chorus)
add_subdirectory(rngs_ensemble)
add_subdirectory(rngs_reverb)
add_subdirectory(wrps)

