add_subdirectory(glm)

target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC
    stb_image/
    glm
)