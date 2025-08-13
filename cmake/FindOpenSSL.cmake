# 尝试先用系统的 OpenSSL
#find_package(OpenSSL QUIET MODULE)

if(NOT OpenSSL_FOUND)
    message(STATUS "OpenSSL not found via system search, using manual config.")

    if(WIN32)
        # Windows 示例路径
        set(OPENSSL_ROOT_DIR "D:/Software/openssl-3.5.1" CACHE PATH "Path to OpenSSL installation")
        set(OPENSSL_INCLUDE_DIR "${OPENSSL_ROOT_DIR}/include")
        set(OPENSSL_LIBRARIES
            "${OPENSSL_ROOT_DIR}/lib/libssl.lib"
            "${OPENSSL_ROOT_DIR}/lib/libcrypto.lib"
        )

    elseif(APPLE)
        set(OPENSSL_ROOT_DIR "/usr/local/opt/openssl@1.1" CACHE PATH "Path to OpenSSL installation")
        set(OPENSSL_INCLUDE_DIR "${OPENSSL_ROOT_DIR}/include")
        set(OPENSSL_LIBRARIES
            "${OPENSSL_ROOT_DIR}/lib/libssl.dylib"
            "${OPENSSL_ROOT_DIR}/lib/libcrypto.dylib"
        )

    elseif(UNIX)
        set(OPENSSL_INCLUDE_DIR "/usr/include/openssl")
        set(OPENSSL_LIBRARIES ssl crypto)
    endif()
else()
    set(OPENSSL_INCLUDE_DIR ${OPENSSL_INCLUDE_DIR})
    set(OPENSSL_LIBRARIES OpenSSL::SSL OpenSSL::Crypto)
endif()
