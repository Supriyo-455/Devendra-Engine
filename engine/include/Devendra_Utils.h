#ifndef _DEVENDRA_UTIL_H
#define _DEVENDRA_UTIL_H

#ifdef __DEVENDRA_UTILS_EXPORTS__
#define DEVENDRA_UTILS_API __declspec(dllexport)
#else
#define DEVENDRA_UTILS_API __declspec(dllimport)
#endif

DEVENDRA_UTILS_API const char* readFile(const char *filePath);

#endif