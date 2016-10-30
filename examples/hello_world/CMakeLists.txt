message(STATUS "Currently in ${CMAKE_CURRENT_SOURCE_DIR}: ${EXAMPLE}")

set(HEADERS

)

set(SOURCES
    main.cxx
)

add_executable(${EXAMPLE} ${SOURCES} ${HEADERS})
target_link_libraries(${EXAMPLE} multimodelfitting)
