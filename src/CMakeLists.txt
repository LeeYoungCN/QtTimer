set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};${CMAKE_ORIGIN_PATH}")

find_package(Origin REQUIRED)

set(RUNTIME Timer)

set(QT_PATH "D:/ProgramFiles/Qt/6.12.0/msvc2022_64")
set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};${QT_PATH}")

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(${RUNTIME}
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
)

target_link_libraries(${RUNTIME} PRIVATE Qt::Core Qt::Widgets)
target_link_libraries(${RUNTIME} PRIVATE Origin::Common Origin::Utils Origin::Container Origin::Logging)

if (WIN32)
    add_custom_command(TARGET ${RUNTIME} POST_BUILD
        COMMAND Qt6::windeployqt $<TARGET_FILE:${RUNTIME}>
        COMMENT "Run windeployqt to copy Qt dlls"
    )
endif()

deploy_origin_post_build(${RUNTIME})
include(GNUInstallDirs)          # 关键：让 CMAKE_INSTALL_BINDIR 有定义
install_origin(bin)

install(TARGETS ${RUNTIME}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET ${RUNTIME}
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)

install(SCRIPT ${deploy_script})
