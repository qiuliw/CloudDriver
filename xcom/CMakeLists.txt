cmake_minimum_required(VERSION 3.22)
project(xcom)

set(CMAKE_CXX_STANDARD 20)

aux_source_directory(. SRC_LIST)

## 必须声明为动态库，默认静态库
add_library(xcom SHARED ${SRC_LIST}
        Logger.h)

## 污染作用域
#include_directories(
#    ${LIBEVENT_INCLUDE_DIR}
#    ${OPENSSL_INCLUDE_DIR}
#)

target_include_directories(xcom
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        PRIVATE
        ${LIBEVENT_INCLUDE_DIR}
        ${OPENSSL_INCLUDE_DIR}
)

target_link_libraries(xcom PRIVATE
    ${OPENSSL_LIBRARIES}
    ${LIBEVENT_LIBRARIES}
)

if (WIN32)
    target_link_libraries(xcom PRIVATE
        ws2_32
    )
    target_compile_definitions(xcom PRIVATE XCOM_EXPORTS)  # 编译导出符号
endif ()

