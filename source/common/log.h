/**
* @file
* @brief
* @author   caiyangang
* <pre><b>email: </b>caiyangang@pku.edu.cn</pre>
* <pre><b>All rights reserved.</b></pre>
* @date 2020/04/13
* @see
*/

#pragma once

#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>
#include <string>

enum LogLevel
{
  LOG_LEVEL_TRC  = 1,
  LOG_LEVEL_DBG  = 2,
  LOG_LEVEL_USER = 3,
  LOG_LEVEL_INF  = 4,
  LOG_LEVEL_WRN  = 5,
  LOG_LEVEL_ERR  = 6,
  LOG_LEVEL_NON  = 7,
};

const char* log_level2str(int level);

void LogSetLevel(LogLevel lvl);
LogLevel LogGetLevel();

typedef void(LogCallback)(int level, const char *file, int line_num, const char *format, va_list args);

void LogSetCallback(LogCallback *cbp, const char* filename, const char* statistics_file_name);
void log_to_console(int level, const char *file, int line_num, const char *format, va_list args);

void LogPrintf(int level, const char *file, int line_num, const char *format, ...);



#define TRC(...) do { \
    if (LogGetLevel() > LOG_LEVEL_TRC) break; \
    LogPrintf(LOG_LEVEL_TRC, __FILE__, __LINE__, __VA_ARGS__);	\
} while (0)

#define DBG(...) do { \
    if (LogGetLevel() > LOG_LEVEL_DBG) break; \
    LogPrintf(LOG_LEVEL_DBG, __FILE__, __LINE__, __VA_ARGS__);	\
} while (0)

#define USE(...) do { \
    if (LogGetLevel() > LOG_LEVEL_USER) break; \
    LogPrintf(LOG_LEVEL_USER, __FILE__, __LINE__, __VA_ARGS__);	\
} while (0)

#define INF(...) do { \
    if (LogGetLevel() > LOG_LEVEL_INF) break; \
    LogPrintf(LOG_LEVEL_INF, __FILE__, __LINE__, __VA_ARGS__);	\
} while (0)

#define WRN(...) do { \
    if (LogGetLevel() > LOG_LEVEL_WRN) break; \
    LogPrintf(LOG_LEVEL_WRN, __FILE__, __LINE__, __VA_ARGS__);	\
} while (0)

#define ERR(...) do { \
    if (LogGetLevel() > LOG_LEVEL_ERR) break; \
    LogPrintf(LOG_LEVEL_ERR, __FILE__, __LINE__, __VA_ARGS__);	\
} while (0)