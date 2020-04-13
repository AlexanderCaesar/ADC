/**
* @file
* @brief
* @author   caiyangang
* <pre><b>email: </b>caiyangang@pku.edu.cn</pre>
* <pre><b>All rights reserved.</b></pre>
* @date 2020/04/13
* @see
*/

#include "log.h"
#include <common.h>

#include <fstream>
#include <time.h>
#include <stdio.h>
#include <algorithm>

#ifdef WIN32
#include "windows.h"
#define snprintf _snprintf
#else
#include <sys/time.h>
#include <unistd.h>
#endif

using namespace std;

#ifdef WIN32
#pragma warning(disable:4996)
#endif

static string default_filename = "codecDemo.log";

LogLevel log_level = LOG_LEVEL_INF;
static LogCallback* log_cb = log_to_console;

#ifdef WIN32
struct tm *localtime_r(time_t* timer, struct tm* tp)
{
  struct tm* temp = localtime(timer);
  if (temp == NULL)
  {
    return NULL;
  }

  *tp = *temp;
  return tp;
}

#define sprintf _sprintf
#endif

const char *log_level2str(int level)
{
  switch (level) {
  case LOG_LEVEL_TRC:
    return "TRC";
  case LOG_LEVEL_DBG:
    return "DBG";
  case LOG_LEVEL_INF:
    return "INF";
  case LOG_LEVEL_WRN:
    return "WRN";
  case LOG_LEVEL_ERR:
    return "ERR";
  case LOG_LEVEL_NON:
    return "NON";
  default:
    break;
  }
  return "UNDEFINED";
}

void get_log_str(char* buf, int max_len, int level, const char *file, int line_num, const char *format, va_list args)
{
  buf[max_len - 1] = 0;
  const char *log_type = log_level2str(level);
#ifdef _WIN32
  SYSTEMTIME wtm;
  GetLocalTime(&wtm);
  int ret = snprintf(buf, max_len - 1, "[os_version %s] %02d:%02d:%02d.%03d %s [%s:%d] ", GetOsVersion(), wtm.wHour, wtm.wMinute, wtm.wSecond, wtm.wMilliseconds, log_type, file, line_num);
#else
  struct  timeval    tv;
  struct  timezone   tz;

  struct tm now_time;
  if (gettimeofday(&tv, &tz) != 0)
    return;

  time_t t = tv.tv_sec;

  if (NULL == localtime_r(&t, &now_time))
    memset(&now_time, 0, sizeof(now_time));

  int ret = snprintf(buf, max_len - 1, "%02d:%02d:%02d.%03d %s [%s:%d] ", now_time.tm_hour, now_time.tm_min, now_time.tm_sec, int(tv.tv_usec / 1000), log_type, file, line_num);

#endif

  if (ret > 0)
  {
    ret += vsnprintf(buf + ret, max_len - ret - 1, format, args);
  }
}

void get_statistics_str(char* buf, int max_len, int level, const char *format, va_list args)
{
  buf[max_len - 1] = 0;

  vsnprintf(buf, max_len - 1, format, args);
}

void log_to_console(int level, const char *file, int line_num, const char *format, va_list args)
{
  char buf[1024 * 16];
  buf[0] = 0;
  get_log_str(buf, sizeof(buf), level, file, line_num, format, args);

  printf("%s\n", buf);
}

class LogFile
{
public:
  LogFile()
  {
    file_name = default_filename;
  }
  int32_t load(const char* filename)
  {
    if (filename != NULL)
    {
      file_name = filename;
    }
    else
    {
      file_name = "";
    }

    if (file.is_open())
    {
      file.close();
    }

    if (file_name.length() > 0)
    {
      file.open(file_name.c_str(), ios::out);

      if (file.is_open())
      {
        return 0;
      }
      else
      {
        return -1;
      }
    }
    return 0;
  }
  int32_t write(const char* data)
  {
    if ((data != NULL) && (file.is_open()))
    {
      file << data << endl;
    }
    return 0;
  }
  int32_t close()
  {
    if (file.is_open())
    {
      file.close();
    }
    return 0;
  }


  std::fstream file;
  std::string file_name;
};

LogFile log_file;
LogFile statistics_file;

void LogSetCallback(LogCallback *cbp, const char* filename, const char* statistics_file_name)
{
  if (cbp)
  {
    log_cb = cbp;
  }
  else
  {
    log_cb = log_to_console;
  }

  log_file.load(filename);
  statistics_file.load(statistics_file_name);
}

void LogSetLevel(LogLevel lvl)
{
  log_level = lvl;
}

LogLevel LogGetLevel()
{
  return log_level;
}

void log_to_file(int level, const char *file, int line_num, const char *format, va_list args)
{
  if (log_file.file.is_open())
  {
    char buf[1024 * 16];
    buf[0] = 0;
    get_log_str(buf, sizeof(buf), level, file, line_num, format, args);
    log_file.write(buf);
  }
}

void statistics_to_file(int level, const char *format, va_list args)
{
  if (statistics_file.file.is_open())
  {
    char buf[1024 * 16];
    buf[0] = 0;
    get_statistics_str(buf, sizeof(buf), level, format, args);
    statistics_file.write(buf);
  }
}

void LogPrintf(int level, const char *file, int line_num, const char *format, ...)
{
  if (level == LOG_LEVEL_USER)
  {
    va_list args1;
    va_start(args1, format);
    statistics_to_file(level, format, args1);
    va_end(args1);
    return;
  }
  const char* left_slash = strrchr(file, '/');
  const char* right_slash = strrchr(file, '\\');

  const char* file_name;
  const char* slash = (std::max)(left_slash, right_slash);

  if (slash == NULL)
  {
    file_name = file;
  }
  else
  {
    file_name = slash + 1;
  }

  va_list args;
  va_start(args, format);
  log_cb(level, file_name, line_num, format, args);
  va_end(args);

  va_list args1;
  va_start(args1, format);
  log_to_file(level, file_name, line_num, format, args1);
  va_end(args1);

}