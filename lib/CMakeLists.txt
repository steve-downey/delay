add_library(lazystream STATIC delay.c++ delayasync.c++ stream.c++ streamasync.c++)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
