set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};${CMAKE_ORIGIN_PATH}")
find_package(Origin REQUIRED)

set(RUNTIME Timer)

set(QT_PATH "D:/ProgramFiles/Qt/6.12.0/msvc2022_64")
set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};${QT_PATH}")

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)


qt_add_executable(${RUNTIME}
        MANUAL_FINALIZATION
        main.cpp
        mainwindow.cpp
        mainwindow.h
)

target_link_libraries(${RUNTIME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(${RUNTIME} PROPERTIES
        ${BUNDLE_ID_OPTION}
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
)



target_link_libraries(${RUNTIME} PUBLIC Origin::common Origin::utils Origin::container Origin::logging)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${RUNTIME})
endif()

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
    add_custom_command(TARGET ${RUNTIME} POST_BUILD
        COMMAND Qt6::windeployqt $<TARGET_FILE:Timer>
        COMMENT "Run windeployqt to copy Qt dlls"
    )
    install(DIRECTORY ${ORIGIN_BIN_DIR}/ DESTINATION bin)
endif()

install(TARGETS ${RUNTIME}
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)
