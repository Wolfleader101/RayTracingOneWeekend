SET(PROJECT_NAME RayTracingOneWeekend)
cmake_minimum_required (VERSION 3.8)

project(${PROJECT_NAME} LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
    string(REGEX REPLACE "/W[0-4]" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    string(REGEX REPLACE "/RTC[0-4]" "/O2 /Ob2" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

endif()

add_subdirectory(src)

# Add source to this project's executable.
add_executable(${PROJECT_NAME} ${SOURCES})

add_subdirectory(lib)

target_link_libraries(${PROJECT_NAME} PUBLIC )


 add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                 COMMAND ${CMAKE_COMMAND} -E create_symlink
                 ${CMAKE_SOURCE_DIR}/images $<TARGET_FILE_DIR:${PROJECT_NAME}>/images)