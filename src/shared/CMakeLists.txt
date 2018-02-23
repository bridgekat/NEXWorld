project(shared)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

add_definitions(-DNEXWORLD_PLUGIN_API_IMPL)

aux_source_directory(./ SOURCE)

add_library(shared STATIC ${SOURCE})
