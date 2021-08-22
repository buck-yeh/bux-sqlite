add_library(buxosqlite STATIC oo_sqlite.cpp)
target_include_directories(buxosqlite PRIVATE ../include ../../bux/include)
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
target_link_libraries(buxosqlite PRIVATE fmt::fmt)
endif()


