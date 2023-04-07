SET(PROJECT_NAME RayTracingOneWeekend)
cmake_minimum_required (VERSION 3.8)

project(${PROJECT_NAME} LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_subdirectory(src)

# Add source to this project's executable.
add_executable(${PROJECT_NAME} ${SOURCES})

# add_subdirectory(lib)

# target_link_libraries(${PROJECT_NAME} PUBLIC )