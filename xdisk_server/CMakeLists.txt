cmake_minimum_required(VERSION 3.22)
project(xdisk_server)

set(CMAKE_CXX_STANDARD 20)

aux_source_directory(. SRC_LIST)

add_executable(xdisk_server ${SRC_LIST})

target_include_directories(xdisk_server
        PUBLIC
#        ${CMAKE_CURRENT_SOURCE_DIR}
        ${OPENSSL_INCLUDE_DIR}
)

target_link_libraries(xdisk_server PRIVATE
    xcom
    ${LIBEVENT_LIBRARIES}
    ws2_32
)