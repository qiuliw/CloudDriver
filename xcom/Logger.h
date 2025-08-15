#pragma once

#include <cstdio>
#include <cstdlib>

#define LOG_INFO(fmt, ...) \
do { \
    printf("[INFO] "); \
    printf(fmt, ##__VA_ARGS__); \
    printf("\n"); \
} while(0)

#define LOG_WARN(fmt, ...) \
do { \
    printf("[WARN] "); \
    printf(fmt, ##__VA_ARGS__); \
    printf("\n"); \
} while(0)

#define LOG_ERROR(fmt, ...) \
do { \
    printf("[ERROR] "); \
    printf(fmt, ##__VA_ARGS__); \
    printf("\n"); \
} while(0)

#define LOG_FATAL(fmt, ...) \
do { \
    printf("[FATAL] "); \
    printf(fmt, ##__VA_ARGS__); \
    printf("\n"); \
} while(0)
