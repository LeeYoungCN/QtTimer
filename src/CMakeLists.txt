
set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};${CMAKE_ORIGIN_PATH}")

find_package(Origin REQUIRED)

set(RUNTIME Timer)

add_executable(${RUNTIME} main.cpp)

target_link_libraries(${RUNTIME} PUBLIC Origin::common Origin::utils Origin::container Origin::logging)

if (WIN32)
    file(GLOB LIBS "${ORIGIN_BIN_DIR}/*.dll")

    add_custom_command(TARGET ${RUNTIME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${LIBS}
        $<TARGET_FILE_DIR:${RUNTIME}>
        COMMAND_EXPAND_LISTS
        COMMENT "Copy ${ORIGIN_LIBS}."
        VERBATIM
    )
    install(DIRECTORY ${ORIGIN_BIN_DIR}/ DESTINATION bin)
endif()

install(TARGETS ${RUNTIME}
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)
