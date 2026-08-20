add_executable(can_node main.c can.c)



target_include_directories(can_node PRIVATE ${PROJECT_SOURCE_DIR}/common)



target_link_libraries(can_node common)