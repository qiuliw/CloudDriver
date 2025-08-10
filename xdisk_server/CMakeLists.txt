cmake_minimum_required(VERSION 3.22)
project(xdisk_server)

set(CMAKE_CXX_STANDARD 20)

aux_source_directory(. SRC_LIST)

add_executable(xdisk_server ${SRC_LIST})

if(WIN32) #windows

    # 指定 libevent 路径
    set(LIBEVENT_DIR "D:/Software/libevent-2.1.12-stable")

    # 指定 OpenSSL 路径
    set(OPENSSL_ROOT_DIR "D:/Software/openssl-3.5.1")

    # 头文件搜索路径
    include_directories(
            "${PROJECT_SOURCE_DIR}/../xcom"
            ${LIBEVENT_DIR}/build_msvc_ssl_shared/include # libevent生成的event-config.h文件所在目录，提供跨平台自动选择不同函数和头文件的功能。是event根据系统自动生成的,必须包含
            ${LIBEVENT_DIR}/include # libevent头文件所在目录，实际供外部调用的头文件
            ${OPENSSL_ROOT_DIR}/include
    )

    target_link_libraries(xdisk_server
            "${PROJECT_SOURCE_DIR}/../lib/xcom.lib"
            "${OPENSSL_ROOT_DIR}/libssl.lib"
            "${LIBEVENT_DIR}/build_msvc_ssl_shared/lib/Release/event.lib"
            ws2_32   # Windows 套接字库
    )

elseif(UNIX AND NOT APPLE) # linux

    find_package(OpenSSL REQUIRED)
    include_directories(
            "${PROJECT_SOURCE_DIR}/../xcom"
            ${OPENSSL_INCLUDE_DIR} #ssl
    )
    
    target_link_libraries(xdisk_server 
            xcom # mingw编译支持模糊匹配链接，msvc不行
            ${OPENSSL_LIBRARIES} #ssl
            event # libevent
    )

endif()