#find_library(LIBEVENT_CORE_LIBRARY NAMES event_core)
#find_library(LIBEVENT_LIBRARY NAMES event)
#find_library(LIBEVENT_EXTRA_LIBRARY NAMES event_extra)

if(LIBEVENT_INCLUDE_DIR AND LIBEVENT_LIBRARY)
    set(LIBEVENT_LIBRARIES
        ${LIBEVENT_LIBRARY}
        ${LIBEVENT_CORE_LIBRARY}
        ${LIBEVENT_EXTRA_LIBRARY}
    )
    set(LIBEVENT_FOUND TRUE)
else()
    set(LIBEVENT_FOUND FALSE)
endif()

if(NOT LIBEVENT_FOUND)
    message(STATUS "LibEvent not found in system paths, using manual config.")

    if(WIN32)
        set(LIBEVENT_DIR "D:/Software/libevent-2.1.12-stable" CACHE PATH "Path to LibEvent")
        set(LIBEVENT_INCLUDE_DIR
            "${LIBEVENT_DIR}/include"
        )
        set(LIBEVENT_LIBRARIES
            "${LIBEVENT_DIR}/lib/event.lib"
            "${LIBEVENT_DIR}/lib/event_core.lib"
            "${LIBEVENT_DIR}/lib/event_extra.lib"
        )

    elseif(APPLE OR UNIX)
        set(LIBEVENT_INCLUDE_DIR "/usr/include")
        set(LIBEVENT_LIBRARIES event event_core event_extra)
    endif()
endif()

