find_package (SQLite3)

if (NOT SQLITE3_FOUND)
    message("SQLite3 not found")
endif ()

add_library(bux-sqlite STATIC oo_sqlite.cpp)

if(DEFINED FETCH_DEPENDEES)
    target_include_directories(bux-sqlite PRIVATE ../include ${SQLITE3_INCLUDE_DIRS})
else()
    target_include_directories(bux-sqlite PRIVATE ../include ../${DEPENDEE_ROOT}/bux/include ${SQLITE3_INCLUDE_DIRS})
endif()
target_link_libraries (bux-sqlite ${SQLITE3_LIBRARIES})
