/**
 * @file log.h
 * @author aris
 * @brief print log
 * @version 0.1
 * @date 2022-02-01
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef __STUDY_SRC_LOG_H__
#define __STUDY_SRC_LOG_H__

#include <string>
#include <iostream>
#include <memory>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <unistd.h>
#include <chrono>

#include "singelton.h"
#include "utils.h"

/**
 * @brief log use 
 * ARIS_LOG_INFO("%v %v", num, message);
 */


#define ARIS_LOG_FMT_DEBUG(fmt, ...) ARIS_LOG_FMT_SIMPLE(aris::LogLevel::Level::DEBUG, fmt, __VA_ARGS__)
#define ARIS_LOG_FMT_INFO(fmt, ...) ARIS_LOG_FMT_SIMPLE(aris::LogLevel::Level::INFO, fmt, __VA_ARGS__)
#define ARIS_LOG_FMT_WARN(fmt, ...) ARIS_LOG_FMT_SIMPLE(aris::LogLevel::Level::WARN, fmt, __VA_ARGS__)
#define ARIS_LOG_FMT_ERROR(fmt, ...) ARIS_LOG_FMT_SIMPLE(aris::LogLevel::Level::ERROR, fmt, __VA_ARGS__)

#define ARIS_LOG_FMT_SIMPLE(level, fmt, ...) \
    aris::SingeltonPtr<aris::LogMgr>::get_instance()->log(level, aris::LogEvent::ptr(new aris::LogEvent(__FILE__, \
        __func__ , __LINE__, getpid(), pthread_self(), 0, std::chrono::system_clock::now(), aris::StringGenerator::format(fmt, __VA_ARGS__))))

namespace aris {

struct LogLevel {
/**
 * @brief use to mark diff log level
 * 
 */
enum class Level {UNKNOWN, TRACE, DEBUG, INFO, WARN, ERROR, FATAL};

/**
 * @brief transform class level to string
 * 
 * @return const std::string 
 */
static std::string level_to_string(Level level);

/**
 * @brief transform msg to string
 * 
 * @param msg 
 * @return Level 
 */
static Level string_to_level(const std::string & msg);
};

class LogEvent : public std::enable_shared_from_this<LogEvent> {
public:
    typedef  std::chrono::time_point<std::chrono::system_clock> TimePoint;
    typedef std::shared_ptr<LogEvent> ptr;
    /**
     * @brief delete default constructor
     * avoid creating non-param obj
     * 
     */
    LogEvent() = delete;

    // now only exist one param, other params use default value
    LogEvent(const std::string & msg);

    virtual~LogEvent() {};

    /**
     * @brief get code info
     * 
     */
    const std::string get_file() { return code_file_; }
    const std::string get_func() { return code_func_; }
    uint32_t get_line() { return code_line_; } 

    /**
     * @brief get execute info
     * 
     */
    uint32_t get_proc_id() { return proc_id_; } 
    uint32_t get_thread_id() { return thread_id_; } 
    uint32_t get_coroutine_id() { return coroutine_id_; } 

    /**
     * @brief 
     * 
     */
    TimePoint get_log_time() { return log_time_; }
    const std::string get_log_msg() { return log_msg_; }

public:
    /**
     * @brief c++11 delegating constructor
     * 
     * @param file 
     */
    LogEvent(const std::string & file, const std::string & func, uint32_t line, 
        uint32_t pid, uint32_t tid, uint32_t cid, TimePoint time, const std::string & msg);

private:
    /**
     * @brief use to mark which file, func and line
     * this log is output
     * 
     */
    std::string code_file_ {};
    std::string code_func_ {};
    uint32_t code_line_ {};

    /**
     * @brief use to mark current process, thread and coroutine
     * 
     */
    uint32_t proc_id_ {0};
    uint32_t thread_id_ {0};
    uint32_t coroutine_id_ {0};

    /**
     * @brief log time and log message
     * 
     */
    TimePoint log_time_ {};
    std::string log_msg_ {};
};

class LogFormatter {
public:
    /**
     * @brief 构造函数
     * @param[in] pattern 格式模板
     * @details 
     *  %m 消息
     *  %p 日志级别
     *  %r 累计毫秒数
     *  %c 日志名称
     *  %t 线程id
     *  %n 换行
     *  %d 时间
     *  %f 文件名
     *  %l 行号
     *  %T 制表符
     *  %F 协程id
     *  %N 线程名称
     *
     *  默认格式 "%d{%Y-%m-%d %H:%M:%S}%T%t%T%N%T%F%T[%p]%T[%c]%T%f:%l%T%m%n"
     */
    typedef std::shared_ptr<LogFormatter> ptr;

    /**
     * @brief Construct a new Log Formatter object
     * 
     * @param pattern 
     */
    LogFormatter(const std::string & pattern = "%d{%Y-%m-%d %H:%M:%S}%T[%p]%T%m%n");

    /**
     * @brief Destroy the Log Formatter object
     * 
     */
    virtual~LogFormatter();

    /**
     * @brief init format
     * 
     * @return * void 
     */
    void init();

    /**
     * @brief Get the format object
     * 
     * @return * const std::string 
     */
    const std::string get_format() const {return log_pattern_;}

public:
    /**
     * @brief 
     * 
     * @param event 
     * @return * const std::string 
     */
    std::string format(LogLevel::Level level, const LogEvent::ptr & event);

    /**
     * @brief 
     * 
     * @param[in, out] os log out 
     * @param event 
     * @return std::ostream& 
     */
    std::ostream & format(std::ostream & os, LogLevel::Level level, const LogEvent::ptr & event);

public:
    class FormatItem {
    public:
    typedef std::shared_ptr<FormatItem> ptr;
    FormatItem(const std::string & name):item_name_(name) {}
    virtual~FormatItem() {}

    virtual std::string format(LogLevel::Level level, const LogEvent::ptr event) = 0;
    virtual std::ostream & format(std::ostream & os, LogLevel::Level level, const LogEvent::ptr event) = 0;

    std::string get_item_name() { return item_name_; }
    protected:
    std::string item_name_ {};
    };

private:
    /// log format
    std::string log_pattern_;
    std::vector<FormatItem::ptr> format_items_;
};

class LogAppender {
public:
    typedef std::shared_ptr<LogAppender> ptr;
    typedef Mutex MutexType;
    LogAppender() { formatter_ = LogFormatter::ptr(new LogFormatter()); }
    virtual~LogAppender() {}

    // op log level
    LogLevel::Level get_log_level();
    void set_log_level(LogLevel::Level level);

    // op log format
    const std::string get_log_format() const;
    void set_log_format(const std::string & format);

    // output log
    virtual void log(LogLevel::Level level, const LogEvent::ptr event) = 0;

protected:
    // set default log level as info
    LogLevel::Level level_ {LogLevel::Level::TRACE};
    // log format
    LogFormatter::ptr formatter_ {nullptr};
    // write mutex
    MutexType mutex_;
}; 

class StdoutLogAppender : public LogAppender {
public:
    typedef std::shared_ptr<StdoutLogAppender> ptr;
    virtual void log(LogLevel::Level level, const LogEvent::ptr event) override;
};

class FileLogAppender : public LogAppender {
public:
    typedef std::shared_ptr<FileLogAppender> ptr;
    virtual void log(LogLevel::Level level, const LogEvent::ptr event) override;

    bool init(const std::string & file);
    virtual~FileLogAppender();

private:
    std::ofstream file_;
};


class Logger {
public:
    typedef std::shared_ptr<Logger> ptr;
    Logger(const std::string & name = "logger");
    virtual~Logger();

    // add log appender
    void addLogAppender(LogAppender::ptr appender);
    void delLogAppender(LogAppender::ptr appender);

    // name 
    void set_name(const std::string & name);
    const std::string get_name();

    // log
    void log(LogLevel::Level level, const LogEvent::ptr event);
private:
    std::string name_ {""};
    std::vector<LogAppender::ptr> appenders_;
};


class LogMgr : public std::enable_shared_from_this<LogMgr> {
public:
    void addLogger(Logger::ptr logger);
    void delLogger(Logger::ptr logger);

    void log(LogLevel::Level level, const LogEvent::ptr event);
    void trace(const LogEvent::ptr event);
    void debug(const LogEvent::ptr event);
    void info(const LogEvent::ptr event);
    void warn(const LogEvent::ptr event);
    void error(const LogEvent::ptr event);
    void fatal(const LogEvent::ptr event);

private:
    std::unordered_map<std::string, Logger::ptr> logger_maps_;
};


};




#endif