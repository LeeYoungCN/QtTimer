# 使用FetchContent模块
include(FetchContent)

# 声明并下载Google Test
FetchContent_Declare(
  Origin
  GIT_REPOSITORY https://github.com/LeeYoungCN/origin.git
  GIT_TAG        6ec52127f2d6af687feda378551f78c6f45ef870
)

FetchContent_MakeAvailable(Origin)
