
set(SOURCES
    term.c
)

add_library(syrup-term ${SOURCES})
target_link_libraries(syrup-term syrup-buffer)
target_include_directories(syrup-term PUBLIC ${PROJECT_SOURCE_DIR}/include)