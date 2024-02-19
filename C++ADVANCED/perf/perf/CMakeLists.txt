
add_compile_options(-Wall -pedantic -Wextra -Wno-sign-compare)
add_compile_options(-Wold-style-cast -Wextra-semi -Woverloaded-virtual -Wzero-as-null-pointer-constant)
if(TREAT_WARNINGS_AS_ERRORS)
  add_compile_options(-Werror -pedantic-errors)
endif()

add_executable(jpeg jpeg.cpp)
target_link_libraries(jpeg benchmark::benchmark)

add_executable(pi pi.cpp)
target_link_libraries(pi benchmark::benchmark)

add_executable(radix_sort radix-sort.cpp)
target_link_libraries(radix_sort benchmark::benchmark)

add_executable(json json.cpp)
target_link_libraries(json benchmark::benchmark)

add_executable(logger logger.cpp)
target_link_libraries(logger benchmark::benchmark)
