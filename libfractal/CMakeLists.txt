include(FetchContent)
FetchContent_Declare(glfw GIT_REPOSITORY https://github.com/glfw/glfw.git)
FetchContent_MakeAvailable(glfw)

file(GLOB FRACTAL_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/*.c
        ${CMAKE_CURRENT_LIST_DIR}/*.h)

add_library(libfractal ${FRACTAL_SOURCES} "${CMAKE_SOURCE_DIR}/extern/glad/glad.h" "${CMAKE_SOURCE_DIR}/extern/glad/glad.c")
target_include_directories(libfractal PUBLIC ${CMAKE_SOURCE_DIR}/extern/)
target_link_libraries(libfractal PUBLIC "glfw")
