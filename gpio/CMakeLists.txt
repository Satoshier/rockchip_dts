add_executable(water_node main.c water.c)

target_include_directories(water_node PRIVATE ${PROJECT_SOURCE_DIR}/common)

target_link_libraries(water_node common)