#pragma once

#define LOG_VERSION "0.1.0"

typedef struct log_Event
{
  va_list ap;
  const char *fmt;
  const char *file;
  struct tm *time;
  void *udata;
  int32 line;
  int32 level;
} log_Event;

typedef void (*log_LogFn)(log_Event *ev);
typedef void (*log_LockFn)(bool32 lock, void *udata);

enum { LOG_TRACE, LOG_DEBUG, LOG_INFO, LOG_WARN, LOG_ERROR, LOG_FATAL };

#define log_trace(...) log_log(LOG_TRACE, __FILE__, __LINE__, __VA_ARGS__)
#define log_debug(...) log_log(LOG_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define log_info(...)  log_log(LOG_INFO,  __FILE__, __LINE__, __VA_ARGS__)
#define log_warn(...)  log_log(LOG_WARN,  __FILE__, __LINE__, __VA_ARGS__)
#define log_error(...) log_log(LOG_ERROR, __FILE__, __LINE__, __VA_ARGS__)
#define log_fatal(...) log_log(LOG_FATAL, __FILE__, __LINE__, __VA_ARGS__)

const char* log_level_string(int32 level);
void log_set_lock(log_LockFn fn, void *udata);
void log_set_level(int32 level);
void log_set_quiet(bool32 enable);
int32 log_add_callback(log_LogFn fn, void *udata, int32 level);
int32 log_add_fp(FILE *fp, int32 level);

void log_log(int32 level, const char *file, int32 line, const char *fmt, ...);