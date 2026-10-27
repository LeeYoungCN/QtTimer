cmake_minimum_required(VERSION 3.14)
project(Timer VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS   ON)
set(CMAKE_VERBOSE_MAKEFILE          OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

option(BUILD_TESTS "Build and run tests."   OFF)
option(ENABLE_ASAN "Enable asan."           OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(FETCHCONTENT_BASE_DIR ${CMAKE_SOURCE_DIR}/external CACHE PATH "下载依赖库的根目录")

if (MSVC)
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /MDd")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /MD")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
    add_compile_options(/permissive- /utf-8 /Zc:__cplusplus)
    if (NOT ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
        add_compile_options(/Zc:preprocessor)
    endif()
endif()

if (MINGW OR CMAKE_COMPILER_IS_GNUCXX OR
    (${CMAKE_CXX_COMPILER_ID} MATCHES "(Apple)?Clang" AND
     ${CMAKE_C_COMPILER_FRONTEND_VARIANT} STREQUAL "GNU"))
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g3 -O0")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3 -O0")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
    add_compile_options("-Wall" "-Wconversion" "-Wextra" "-pedantic")
endif()

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Default CMAKE_BUILD_TYPE Debug" FORCE)
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(DEBUG_MODE)
else()
    add_compile_definitions(NDEBUG_MODE)
endif()

include(${CMAKE_SOURCE_DIR}/cmake/get_cmake_preset.cmake)

if (${CMAKE_INSTALL_PREFIX} STREQUAL "C:/Program Files (x86)/${PROJECT_NAME}" OR
    ${CMAKE_INSTALL_PREFIX} STREQUAL "/usr/local")
    set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/out/install/${CMAKE_PRESET}" CACHE STRING "Default CMAKE_INSTALL_PREFIX out/install/${CMAKE_PRESET}" FORCE)
endif()

if (ENABLE_ASAN)
    if (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
        if(MSVC)
            add_compile_options(/fsanitize=address /Zi)
            add_link_options(/fsanitize=address)
            # 关闭不兼容项
            string(REGEX REPLACE "/RTC(su|[1su])" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
            string(REGEX REPLACE "/RTC(su|[1su])" "" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
            set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /INCREMENTAL:NO")
            set(CMAKE_THIRD_PARTY_PATH "")
        else()
            message(WARNING "ASan is not supported on this platform/compiler.")
        endif()
    else()
        if (CMAKE_COMPILER_IS_GNUCXX OR
            (${CMAKE_CXX_COMPILER_ID} MATCHES "(Apple)?Clang"))
            add_compile_options(-fsanitize=address -fno-omit-frame-pointer)
            add_link_options(-fsanitize=address)
        else()
            message(WARNING "ASan is not supported on this platform/compiler.")
        endif()
    endif()
endif()

include(${CMAKE_SOURCE_DIR}/cmake/cmake_configure_info.cmake)

include(${CMAKE_SOURCE_DIR}/cmake/content/Origin.cmake)
if (BUILD_TESTS)
    enable_testing()
    add_compile_definitions(ENABLE_TEST)
endif()

add_subdirectory(src)
