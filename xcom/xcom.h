#pragma once

#if defined(_WIN32) || defined(_WIN64)
    #ifdef XCOM_EXPORTS
        #define XCOM_API __declspec(dllexport)
    #else
        #define XCOM_API __declspec(dllimport)
    #endif
#else
    #define MYLIB_API // 空的导出宏
#endif