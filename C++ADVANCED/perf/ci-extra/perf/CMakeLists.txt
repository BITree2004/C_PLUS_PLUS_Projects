
add_executable(base_jpeg jpeg.cpp)
target_link_libraries(base_jpeg benchmark::benchmark)

add_executable(base_pi pi.cpp)
target_link_libraries(base_pi benchmark::benchmark)

add_executable(base_radix_sort radix-sort.cpp)
target_link_libraries(base_radix_sort benchmark::benchmark)

add_executable(base_json json.cpp)
target_link_libraries(base_json benchmark::benchmark)

add_executable(base_logger logger.cpp)
target_link_libraries(base_logger benchmark::benchmark)
