cmake_minimum_required(VERSION 3.22)

project(CloudDriver)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)  # 生成 compile_commands.json

# 设置项目可执行文件输出的路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
# 设置静态库库库文件和动态库的导入库输出的路径
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/lib)
# 动态库输出路径(与可执行文件同路径或在系统变量中)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/bin)

# 添加cmake模块搜索路径
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

# 找第三方库（通过自定义 FindXXX.cmake）
find_package(OpenSSL REQUIRED)
find_package(Libevent REQUIRED)

add_subdirectory(xcom)
if(WIN32)
add_subdirectory(xdisk_gui) # 客户端只编译WIN的
endif()
add_subdirectory(xdisk_server)
