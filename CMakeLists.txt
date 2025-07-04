cmake_minimum_required(VERSION 3.10)

project(machi VERSION 1.0.0 LANGUAGES C CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/glad/include)

# Adding Glad src files (assuming they're in lib folder)
file(GLOB GLAD_SRC "${CMAKE_CURRENT_SOURCE_DIR}/lib/glad/src/*.c")

# Create your executable
add_executable(${PROJECT_NAME} src/main.cpp ${GLAD_SRC})

# Link libraries
target_link_libraries(${PROJECT_NAME}
    glfw
    glm::glm
    ${CMAKE_DL_LIBS}  # For dynamic loading (needed by GLAD)
)

# Platform-specific setup
if(WIN32)
  # Windows: Link with OpenGL32
  target_link_libraries(${PROJECT_NAME} opengl32)

elseif(APPLE)
  # macOS: Special handling for OpenGL conflicts
  target_compile_definitions(${PROJECT_NAME} PRIVATE
        GL_SILENCE_DEPRECATION    # Silence macOS OpenGL deprecation warnings
        GLFW_INCLUDE_NONE        # Prevent GLFW from auto-including OpenGL headers
    )
  target_link_libraries(${PROJECT_NAME} "-framework OpenGL")

else()
  # Linux: Link with OpenGL
  target_link_libraries(${PROJECT_NAME} GL)
endif()
