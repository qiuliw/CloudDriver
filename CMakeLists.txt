cmake_minimum_required(VERSION 3.29)

project(CloudDriver)

set(CMAKE_CXX_STANDARD 20)

# 设置项目可执行文件输出的路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
# 设置静态库库库文件输出的路径
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
# 动态库输出路径(与可执行文件同路径或在系统变量中)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# 添加头文件搜索路径
include_directories(${PROJECT_SOURCE_DIR}/xcom)

# 设置库文件搜索路径
link_directories(${PROJECT_SOURCE_DIR}/lib)

add_subdirectory(xcom)
add_subdirectory(xdisk_gui)
add_subdirectory(xdisk_server)