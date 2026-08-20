add_executable(pwm_node main.c pwm.c)

target_include_directories(pwm_node PRIVATE ${PROJECT_SOURCE_DIR}/common)

target_link_libraries(pwm_node common)