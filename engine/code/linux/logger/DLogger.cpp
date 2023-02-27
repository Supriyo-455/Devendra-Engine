#include "DLogger.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

namespace Devendra
{
    const char *getLogLevel(LOG_LEVEL log)
    {
        switch (log)
        {
        case INFO:
            return "Info";
        case WARN:
            return "Warn";
        case FATAL:
            return "Fatal";
        case TRACE:
            return "Trace";
        default:
            return "log";
        }
    }

    void log(LOG_LEVEL tag, const char *message)
    {
        time_t now;
        time(&now);
        printf("%s [%s]: %s\n", strtok(ctime(&now), "\n"), getLogLevel(tag), message);
    }
}