
set(SOURCES
    term.c
    colors.c
    #form.c
    input.c
    list.c
    line-edit.c
    list-edit.c
    confirm.c
    array-list.c
    private.c
)

add_library(syrup-term ${SOURCES})
target_link_libraries(syrup-term PUBLIC ${SYRUP_BUFFER_LIBRARIES})
target_include_directories(syrup-term PUBLIC ${PROJECT_SOURCE_DIR}/include ${SYRUP_BUFFER_INCLUDE_DIRS})