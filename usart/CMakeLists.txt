add_executable(uart_node main.c usart.c)

target_include_directories(uart_node PRIVATE ${PROJECT_SOURCE_DIR}/common)

target_link_libraries(uart_node common)