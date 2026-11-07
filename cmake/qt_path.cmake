include_guard(GLOBAL)

# Qt 的两套 Windows 二进制 ABI 不通用：msvc2022_64 是 MSVC ABI，mingw_64 是 MinGW ABI。
# 选错会在链接期炸成一片符号未定义，所以这里按编译器身份挑目录。
# 想完全接管，直接 -DQT_PATH=<Qt 目录> 即可，下面的判断全部跳过。
set(QT_MSVC_LIB_PATH  "D:/ProgramFiles/Qt/6.12.0/msvc2022_64" CACHE PATH "Qt for MSVC")
set(QT_MINGW_LIB_PATH "D:/ProgramFiles/Qt/6.12.0/mingw_64"     CACHE PATH "Qt for MinGW")

if (QT_PATH)
    # 已显式指定，尊重之
elseif (NOT WIN32)
    # 非 Windows 不猜路径，交给 find_package 走系统 Qt
    message(STATUS "非 Windows 平台且未指定 QT_PATH，将使用系统 Qt")
elseif (MSVC)
    # cl.exe；clang-cl.exe 也会命中这里
    set(QT_PATH "${QT_MSVC_LIB_PATH}")
elseif (MINGW)
    # gcc.exe / g++.exe（MinGW Makefiles）
    set(QT_PATH "${QT_MINGW_LIB_PATH}")
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    # clang 一个编译器多种 ABI，只能看它伪装成谁：
    #   --target=x86_64-pc-windows-msvc  → CMAKE_CXX_SIMULATE_ID = MSVC
    #   --target=x86_64-w64-mingw32      → CMAKE_CXX_SIMULATE_ID 为空
    if (CMAKE_CXX_SIMULATE_ID STREQUAL "MSVC")
        set(QT_PATH "${QT_MSVC_LIB_PATH}")
    else()
        set(QT_PATH "${QT_MINGW_LIB_PATH}")
    endif()
else()
    message(FATAL_ERROR
        "无法判断该用哪套 Qt（编译器='${CMAKE_CXX_COMPILER_ID}'），请指定 -DQT_PATH=<Qt 目录>")
endif()

if (QT_PATH)
    if (NOT EXISTS "${QT_PATH}/lib/cmake/Qt6")
        message(FATAL_ERROR "QT_PATH='${QT_PATH}' 下没有 lib/cmake/Qt6，请检查 Qt 安装路径")
    endif()
    message(STATUS "QT_PATH: ${QT_PATH}")
endif()
