cmake_minimum_required(VERSION 3.22.1)

project(DevendraRayTracer)

add_library(${PROJECT_NAME} src/Ray.cpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../DevendraMath)

target_include_directories(
    ${PROJECT_NAME} 
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../DevendraMath
)

target_link_directories(
    ${PROJECT_NAME} 
    PRIVATE DevendraMath
)

target_link_libraries(
    ${PROJECT_NAME} 
    DevendraMath
)