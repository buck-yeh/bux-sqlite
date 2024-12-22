find_package (SQLite3)

add_library(bux-sqlite STATIC oo_sqlite.cpp)

if (SQLITE3_FOUND)
    include_directories(${SQLITE3_INCLUDE_DIRS})
    target_link_libraries (bux-sqlite ${SQLITE3_LIBRARIES})
else()
    message("SQLite3 not found")
endif (SQLITE3_FOUND)

if(DEFINED FETCH_DEPENDEES)
    target_include_directories(bux-sqlite PRIVATE ../include)
else()
    target_include_directories(bux-sqlite PRIVATE ../include ../${DEPENDEE_ROOT}/bux/include)
endif()
