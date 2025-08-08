cmake_minimum_required(VERSION 3.29)
project(xcom)

set(CMAKE_CXX_STANDARD 20)

aux_source_directory(. SRC_LIST)

add_library(xcom SHARED ${SRC_LIST}
        xfile_server_task.cpp
        xfile_server_task.h)

# 指定 libevent 路径
set(LIBEVENT_DIR "D:/Software/libevent-2.1.12-stable")

# 指定 OpenSSL 路径
set(OPENSSL_ROOT_DIR "D:/Software/openssl-3.5.1")

# 头文件搜索路径
include_directories(
        ${LIBEVENT_DIR}/build_msvc_ssl_shared/include # libevent生成的event-config.h文件所在目录，提供跨平台自动选择不同函数和头文件的功能。是event根据系统自动生成的,必须包含
        ${LIBEVENT_DIR}/include # libevent头文件所在目录，实际供外部调用的头文件
        ${OPENSSL_ROOT_DIR}/include
)

# 链接 libevent
target_link_libraries(xcom
    "${OPENSSL_ROOT_DIR}/libssl.lib"
    "${LIBEVENT_DIR}/build_msvc_ssl_shared/lib/Release/event.lib"
    ws2_32   # Windows 套接字库
)

# 导出动态库
target_compile_definitions(xcom
        PRIVATE XCOM_EXPORTS
)