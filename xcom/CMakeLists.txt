cmake_minimum_required(VERSION 3.31)
project(xcom)

set(CMAKE_CXX_STANDARD 20)

aux_source_directory(. SRC_LIST)

add_library(xcom SHARED ${SRC_LIST})

# link libevent
if(WIN32)
    set(LIBEVENT_ROOT_DIR "D:/Software/libevent-2.1.12-stable")
    include_directories("${LIBEVENT_ROOT_DIR}/include" "${LIBEVENT_ROOT_DIR}/build/include")
    target_link_directories(xcom PUBLIC "${LIBEVENT_ROOT_DIR}/build/lib")
    target_link_libraries(xcom PRIVATE event ws2_32)
else()
    find_package(Libevent REQUIRED)
    include_directories(${LIBEVENT_INCLUDE_DIRS})
    target_link_libraries(xcom PRIVATE Libevent::libevent)
endif()

# 动态库
target_compile_definitions(xcom
        PRIVATE XCOM_EXPORTS
)