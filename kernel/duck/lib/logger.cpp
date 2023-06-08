#include <cstdarg>

#include <inc/logger.hpp>
#include <inc/timer.hpp>
#include <inc/x86_64.hpp>

using Timer::secf_since_epoch;
using x86_64::rdtsc;

namespace Logger {
LogLevel log_level;

static const char *get_prefix(char name) {
    switch (name) {
    case 'F':
        return "FATAL";
    case 'E':
        return "ERROR";
    case 'I':
        return "INFO";
    case 'W':
        return "WARN";
    case 'D':
        return "DEBUG";
    default:
        return "unknown";
    }
}

static bool logger_in_use = false;

TimedLogger::TimedLogger(char name, bool mute) : name(name), mute(mute) {
    if (mute) return;

    if (!logger_in_use) {
        logger_in_use = true;
    }

    double tm = secf_since_epoch();
    if (tm != -1) {
        printf("[%.6lf]", tm);
    } else {
        printf("[tsc %lu]", rdtsc());
    }

    printf("[%s] ", get_prefix(name));
}

TimedLogger::TimedLogger(const TimedLogger &logger) : name(logger.name), mute(logger.mute) {}

TimedLogger::~TimedLogger() {
    if (mute) return;

    fflush(stdout);

    if (logger_in_use) {
        logger_in_use = false;
    }

    putchar('\n');
}

void set_log_level(LogLevel level) { log_level = level; }

void init() { set_log_level(LL_DEBUG); }

TimedLogger get_logger(LogLevel level, char name) { return TimedLogger(name, level > log_level); }

TimedLogger LFATAL() { return get_logger(LL_FATAL, 'F'); }

TimedLogger LERROR() { return get_logger(LL_ERROR, 'E'); }

TimedLogger LWARN() { return get_logger(LL_WARN, 'W'); }

TimedLogger LINFO() { return get_logger(LL_INFO, 'I'); }

TimedLogger LDEBUG() { return get_logger(LL_DEBUG, 'D'); }

void LFATAL(const char *fmt, ...) {
    if (LL_FATAL > log_level) return;
    auto logger = LFATAL();
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
}

void LERROR(const char *fmt, ...) {
    if (LL_ERROR > log_level) return;
    auto logger = LERROR();
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
}

void LWARN(const char *fmt, ...) {
    if (LL_WARN > log_level) return;
    auto logger = LWARN();
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
}

void LINFO(const char *fmt, ...) {
    if (LL_INFO > log_level) return;
    auto logger = LINFO();
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
}

void LDEBUG(const char *fmt, ...) {
    if (LL_DEBUG > log_level) return;
    auto logger = LDEBUG();
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
}
} // namespace Logger
