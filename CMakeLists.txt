cmake_minimum_required(VERSION 3.22.1)

project(Devendra VERSION 1.0)

configure_file(DevendraConfig.h.in DevendraConfig.h)

set(LINUXDIR engine/code/linux)

add_executable(
    ${PROJECT_NAME} 
    ${LINUXDIR}/Devendra.cpp
    ${LINUXDIR}/logger/DLogger.cpp
    ${LINUXDIR}/utils/DUtils.cpp
)

target_include_directories(
    ${PROJECT_NAME} 
    PUBLIC ${PROJECT_BINARY_DIR}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)

add_subdirectory(DevendraMath)
add_subdirectory(DevendraRayTracer)
add_subdirectory(Sandbox)

# DOWNLOAD ALL THE SUBMODULES
find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
# update submodules as needed
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if(GIT_SUBMODULE)
        message(STATUS "Submodule update")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        RESULT_VARIABLE GIT_SUBMODULE_RESULT)
        if(NOT GIT_SUBMODULE_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMODULE_RESULT}, please checkout submodules")
        endif()
    endif()
endif()

# CHECK ALL THE SUBMODULES
if(NOT EXISTS "${PROJECT_SOURCE_DIR}/thirdparty/glfw/CMakeLists.txt")
    message(FATAL_ERROR "The glfw submodules was not downloaded! GIT_SUBMODULE was turned off or failed. Please check submodules")
endif()

add_subdirectory(thirdparty/glfw)

target_include_directories(
    ${PROJECT_NAME} 
    PUBLIC DevendraMath/include
    PUBLIC DevendraRayTracer/include
    PUBLIC Sandbox/include 
    PUBLIC thirdparty/glfw/include
)

target_link_directories(
    ${PROJECT_NAME} 
    PRIVATE DevendraMath
    PRIVATE DevendraRayTracer
    PRIVATE Sandbox
    PRIVATE thirdparty/glfw/src
)

target_link_libraries(
    ${PROJECT_NAME} 
    DevendraMath
    DevendraRayTracer
    Sandbox
    glfw
)