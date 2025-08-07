#pragma once

#ifdef XCOM_STATIC // 静态库不用加导出宏

#else // 动态库的WIN平台头文件需要加导出宏
    #if defined(_WIN32) || defined(_WIN64)
        #ifdef XCOM_EXPORTS
            #define XCOM_API __declspec(dllexport)
        #else
            #define XCOM_API __declspec(dllimport)
        #endif
    #else
        #define XCOM_API // 空的导出宏
    #endif
#endif