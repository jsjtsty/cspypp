#pragma once

#include <log4cplus/log4cplus.h>

extern log4cplus::Logger logger;

#define LOG_ERROR(logEvent) LOG4CPLUS_ERROR(logger, logEvent)
#define LOG_WARN(logEvent) LOG4CPLUS_WARN(logger, logEvent)
#define LOG_INFO(logEvent) LOG4CPLUS_INFO(logger, logEvent)
#define LOG_DEBUG(logEvent) LOG4CPLUS_DEBUG(logger, logEvent)
#define LOG_TRACE(logEvent) LOG4CPLUS_TRACE(logger, logEvent)
