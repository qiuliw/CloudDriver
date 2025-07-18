cmake_minimum_required(VERSION 3.29)
project(xdisk_server)

set(CMAKE_CXX_STANDARD 20)

aux_source_directory(. SRC_LIST)

add_executable(xdisk_server ${SRC_LIST})

# link libevent
if(WIN32)
    set(LIBEVENT_ROOT_DIR "D:/Software/libevent-2.1.12-stable")
    include_directories("${LIBEVENT_ROOT_DIR}/include" "${LIBEVENT_ROOT_DIR}/build/include")
    target_link_directories(xdisk_server PUBLIC "${LIBEVENT_ROOT_DIR}/build/lib")
    target_link_libraries(xdisk_server PRIVATE event ws2_32)
else()
    find_package(Libevent REQUIRED)
    include_directories(${LIBEVENT_INCLUDE_DIRS})
    target_link_libraries(xdisk_server PRIVATE Libevent::libevent)
endif()

# link xcom
target_link_libraries(xdisk_server PRIVATE xcom)