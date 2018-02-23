project(server)

add_definitions(-DNEXWORLD_PLUGIN_API_IMPL)
add_definitions(-DNEXWORLD_PLUGIN_API_SERVER)

aux_source_directory(./ SOURCE)

set(INCLUDE "../shared/")

add_executable(server ${SOURCE})
target_include_directories(server PUBLIC ${INCLUDE})
target_link_libraries(server shared)
