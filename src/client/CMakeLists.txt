project(client)

add_definitions(-DNEXWORLD_PLUGIN_API_IMPL)
add_definitions(-DNEXWORLD_PLUGIN_API_CLIENT)

aux_source_directory(./ SOURCE)

# Set search paths
set(DEPS_INCLUDE "" CACHE PATH "The path to depended includes")
set(DEPS_LIB "" CACHE PATH "The path to depended libraries")
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${DEPS_INCLUDE} ${DEPS_LIB})
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_ROOT_PATH}/cmake)

# Link OpenGL & GLEW
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
set(DEPS_INCLUDE ${DEPS_INCLUDE} ${GLEW_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIR})
set(DEPS_LIB ${DEPS_LIB} ${GLEW_LIBRARIES} ${OPENGL_gl_LIBRARY})

# Link SDL2
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
set(DEPS_INCLUDE ${DEPS_INCLUDE} ${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR})
set(DEPS_LIB ${DEPS_LIB} ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY})

# Link libdl
if(UNIX)
    set(DEPS_LIB ${DEPS_LIB} dl)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BINARY_DIR})

add_library(client SHARED ${SOURCE})
target_include_directories(client PUBLIC ../shared/ ${DEPS_INCLUDE})
target_link_libraries(client shared ${DEPS_LIB})

add_executable(clientexec ../clientexec.cpp)
target_link_libraries(clientexec client)
