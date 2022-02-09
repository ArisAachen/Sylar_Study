#include "log.h"

#include <sstream>

namespace aris {


/**
 * @brief transform level to string
 * 
 * @param level 
 * @return const std::string 
 */
const std::string LogLevel::level_to_string(LogLevel::Level level) {
    switch (level) {
#define XX(name) \
    case LogLevel::Level::name: \
        return #name;

    XX(TRACE);
    XX(DEBUG);
    XX(INFO);
    XX(WARN);
    XX(FATAL);
    default:
        break;
    }

    // default return unknown string
    return "UNKNOWN";
}

/**
 * @brief transform string to level
 * 
 * @param msg 
 * @return * const Level 
 */
const LogLevel::Level LogLevel::string_to_level(const std::string & msg) {

#define XX(name) \
    if (msg == #name) \
        return LogLevel::Level::name;

    XX(TRACE);
    XX(DEBUG);
    XX(INFO);
    XX(WARN);
    XX(FATAL);
    return Level::UNKNOWN;
}

LogEvent::LogEvent(const std::string & msg) {}

/**
 * @brief private constructor use to delegate other constructor
 * 
 * @param file file name
 * @param func func name
 * @param line log line
 * @param pid current proc id
 * @param tid current thread id
 * @param cid current coroutine id
 * @param time current time
 * @param msg current message
 */
LogEvent::LogEvent(const std::string & file, const std::string & func, uint32_t line, 
    uint32_t pid, uint32_t tid, uint32_t cid, uint64_t time, const std::string & msg):
    code_file_(file), code_func_(func), code_line_(line), 
    proc_id_(pid), thread_id_(tid), coroutine_id_(cid), 
    log_time_(time) {
    // ARIS_LOG_INFO("%v %v", num, message);
    // should check if message is valid, 
    // 1. check if count of %v is equal with full params
    

}


class StringFormatItem : public LogFormatter::FormatItem {
public:
    using LogFormatter::FormatItem::FormatItem;
    ~StringFormatItem() {}

    virtual std::string format(const LogEvent::ptr & event) override {
        return this->get_item_name();
    }

    virtual std::ostream & format(std::ostream & os, const LogEvent::ptr & event) override  {
        return os << this->get_item_name();
    }
};


// %f
class FilenameFormatItem : public LogFormatter::FormatItem { 
public:
    using LogFormatter::FormatItem::FormatItem;
    ~FilenameFormatItem() {}

    virtual std::string format(const LogEvent::ptr & event) override {
        return event->get_file();
    }

    virtual std::ostream & format(std::ostream & os, const LogEvent::ptr & event) override  {
        return os << event->get_file();
    }
};

class FileLineFormatItem : public LogFormatter::FormatItem { 
public:
    using LogFormatter::FormatItem::FormatItem;
    ~FileLineFormatItem() {}

    virtual std::string format(const LogEvent::ptr & event) override {
        std::stringstream stream;
        stream << event->get_line();
        return stream.str();
    }

    virtual std::ostream & format(std::ostream & os, const LogEvent::ptr & event) override  {
        return os << event->get_line();
    }    
}; 


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



LogFormatter::LogFormatter(const std::string & pattern):
    log_pattern_(pattern) {

}

/**
 * @brief init log format
 * 
 */
void LogFormatter::init() {



}




    
}
