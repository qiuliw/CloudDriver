cmake_minimum_required(VERSION 3.29)

project(CloudDriver)

set(CMAKE_CXX_STANDARD 20)

# 设置项目可执行文件输出的路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
# 设置静态库库库文件和动态库的导入库输出的路径
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/lib)
# 动态库输出路径(与可执行文件同路径或在系统变量中)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/bin)

add_subdirectory(xcom)
add_subdirectory(xdisk_gui)  
add_subdirectory(xdisk_server)

