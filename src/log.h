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
#include <vector>


/**
 * @brief log use 
 * ARIS_LOG_INFO("%v %v", num, message);
 */


namespace aris {

struct LogLevel {
/**
 * @brief use to mark diff log level
 * 
 */
enum class Level {UNKNOWN, TRACE, DEBUG, INFO, WARN, FATAL};

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

class LogEvent {
public:
    typedef std::shared_ptr<LogEvent> ptr;
    /**
     * @brief delete default constructor
     * avoid creating non-param obj
     * 
     */
    LogEvent() = delete;

    // now only exist one param, other params use default value
    LogEvent(const std::string & msg);

    virtual~LogEvent();

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
    uint64_t get_log_time() { return log_time_; }
    const std::string get_log_msg() { return log_msg_; }

private:
    /**
     * @brief c++11 delegating constructor
     * 
     * @param file 
     */
    LogEvent(const std::string & file, const std::string & func, uint32_t line, 
        uint32_t pid, uint32_t tid, uint32_t cid, uint64_t time, const std::string & msg);

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
    uint64_t log_time_ {0};
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
    LogFormatter(const std::string & pattern);

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
    const std::string get_format() const;

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
    friend std::ostream & format(std::ostream & os, LogLevel::Level level, const LogEvent::ptr & event);

public:
    class FormatItem {
    public:
    FormatItem(const std::string & name):item_name_(name) {}
    virtual~FormatItem() {}

    virtual std::string format(LogLevel::Level level, const LogEvent::ptr & event) = 0;
    virtual std::ostream & format(std::ostream & os, LogLevel::Level level, const LogEvent::ptr & event) = 0;

    std::string get_item_name() { return item_name_; }
    protected:
    std::string item_name_ {};
    };

private:
    /// log format
    std::string log_pattern_;
    std::vector<FormatItem> format_items_;
};


class LogAppender {

}; 


class Logger {

};


class LogMgr {

};


};




#endif