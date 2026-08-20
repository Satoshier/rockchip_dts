
add_library(common common.c)

target_include_directories(common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(common PRIVATE ini)

add_library(ini ini.c)

target_include_directories(ini PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})