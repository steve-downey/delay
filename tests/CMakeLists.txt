include_directories(${gtest_SOURCE_DIR}/include)
set(test_sources
  test1.c++
  delay.t.c++
  delayasync.t.c++
  stream.t.c++
  streamasync.t.c++
)
add_executable(lazyTest ${test_sources})
target_link_libraries(lazyTest gtest_main lazystream)
add_test(NAME DelayTest COMMAND lazyTest)
