###############################
# pure data external
project(grids)


set(GRDS_SRC
        grids.cpp
        )


include_directories(
        "${PROJECT_SOURCE_DIR}/../mi"
        "${PROJECT_SOURCE_DIR}/.."
)


if (APPLE)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-narrowing -Wno-constant-conversion") 
elseif (UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-narrowing -Wno-constant-conversion")
    if (${CMAKE_SYSTEM_PROCESSOR} MATCHES "^arm")
        # if (BELA)
        #     message(STATUS "BELA optimized")
        # else ()
        #     message(STATUS "non BELA optimized")
        # endif (BELA)
    else ()
        # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}") 
    endif ()
elseif(WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-narrowing -Wno-constant-conversion")
endif (APPLE)


add_library(grids SHARED ${GRDS_SRC})

target_link_libraries(grids MILib)

### setup for pure data

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "grids")


set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")
if (${APPLE})
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-undefined dynamic_lookup")
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".pd_darwin")
elseif (${UNIX})
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".pd_linux")
elseif (${WIN32})
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".dll")
    find_library(PD_LIBRARY NAMES pd HINTS ${PD_CMAKE_PATH})
    target_link_libraries(${PROJECT_NAME} ${PD_LIBRARY})
endif ()

# Removes some warning for Microsoft Visual C.
if (${MSVC})
    target_compile_definitions(${PROJECT_NAME} PRIVATE PD_INTERNAL)
    set_property(TARGET ${PROJECT_NAME} APPEND_STRING PROPERTY COMPILE_FLAGS "/D_CRT_SECURE_NO_WARNINGS /wd4091 /wd4996")
endif ()

