project(mainplugin)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BINARY_DIR}Plugins/)

add_definitions(-DNEXWORLD_PLUGIN_API_CLIENT)

aux_source_directory(./ SOURCE)

# Server not included yet

add_library(mainplugin MODULE ${SOURCE})
target_include_directories(mainplugin PUBLIC ../shared ../client)
target_link_libraries(mainplugin client)
