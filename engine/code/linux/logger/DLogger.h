#pragma once

namespace Devendra
{
    typedef enum LOG_LEVEL
    {
        TRACE = 0,
        INFO,
        WARN,
        ERROR,
        FATAL
    } LOG_LEVEL;

    void log(LOG_LEVEL tag, const char *message);
}
