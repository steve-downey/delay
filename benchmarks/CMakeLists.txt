
add_executable(benchmark_lazy benchmark_lazy.b.c++)
target_link_libraries(benchmark_lazy benchmark lazystream ${CMAKE_THREAD_LIBS_INIT})

add_test(benchmark benchmark_lazy --benchmark_min_time=0.01)
