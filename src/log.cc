#include "log.h"

#include <sstream>
#include <ctime>
#include <cassert>

namespace aris {


/**
 * @brief transform level to string
 * 
 * @param level 
 * @return const std::string 
 */
std::string LogLevel::level_to_string(LogLevel::Level level) {
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
LogLevel::Level LogLevel::string_to_level(const std::string & msg) {

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

    virtual std::string format(LogLevel::Level level, const LogEvent::ptr & event) override {
        return this->get_item_name();
    }

    virtual std::ostream & format(std::ostream & os, LogLevel::Level level, const LogEvent::ptr & event) override  {
        return os << this->get_item_name();
    }
};


// %f
class FilenameFormatItem : public LogFormatter::FormatItem { 
public:
    using LogFormatter::FormatItem::FormatItem;
    ~FilenameFormatItem() {}

    virtual std::string format(LogLevel::Level level, const LogEvent::ptr & event) override {
        return event->get_file();
    }

    virtual std::ostream & format(std::ostream & os, LogLevel::Level level, const LogEvent::ptr & event) override  {
        return os << event->get_file();
    }
};

// %l
class FileLineFormatItem : public LogFormatter::FormatItem { 
public:
    using LogFormatter::FormatItem::FormatItem;
    ~FileLineFormatItem() {}

    virtual std::string format(LogLevel::Level level, const LogEvent::ptr & event) override {
        std::stringstream stream;
        stream << event->get_line();
        return stream.str();
    }

    virtual std::ostream & format(std::ostream & os, LogLevel::Level level, const LogEvent::ptr & event) override  {
        return os << event->get_line();
    }    
}; 

// %m
class MessageFormatItem : public LogFormatter::FormatItem { 
public:
    using LogFormatter::FormatItem::FormatItem;
    ~MessageFormatItem() {}

    virtual std::string format(LogLevel::Level level, const LogEvent::ptr & event) override {
        return event->get_log_msg();
    }

    virtual std::ostream & format(std::ostream & os, LogLevel::Level level, const LogEvent::ptr & event) override  {
        return os << event->get_log_msg();
    }
};

// %p
class LogLevelFormatItem : public LogFormatter::FormatItem { 
public:
    using LogFormatter::FormatItem::FormatItem;
    ~LogLevelFormatItem() {}

    virtual std::string format(LogLevel::Level level, const LogEvent::ptr & event) override {
        return LogLevel::level_to_string(level);
    }

    virtual std::ostream & format(std::ostream & os, LogLevel::Level level, const LogEvent::ptr & event) override  {
        return os << LogLevel::level_to_string(level);
    }
};

// %d
class DateTimeFormatItem :  public LogFormatter::FormatItem { 
public:
    using LogFormatter::FormatItem::FormatItem;
    ~DateTimeFormatItem() {}

    virtual std::string format(LogLevel::Level level, const LogEvent::ptr & event) override {
        // TODO: can use c+11 chrono lib here
        time_t seconds = event->get_log_time();
        struct tm* local_time = localtime(&seconds);
        char buf[64];
        strftime(buf, sizeof(buf), item_name_.c_str(), local_time);
        std::stringstream stream;
        stream << buf;
        return stream.str();
    }

    virtual std::ostream & format(std::ostream & os, LogLevel::Level level, const LogEvent::ptr & event) override  {
        time_t seconds = event->get_log_time();
        struct tm* local_time = localtime(&seconds);
        char buf[64];
        strftime(buf, sizeof(buf), item_name_.c_str(), local_time);
        return os << buf;
    }
};

// %c
class ProcIdFormatItem : public LogFormatter::FormatItem { 
public:
    using LogFormatter::FormatItem::FormatItem;
    ~ProcIdFormatItem() {}

    virtual std::string format(LogLevel::Level level, const LogEvent::ptr & event) override {
        std::stringstream stream;
        stream << event->get_proc_id();
        return stream.str();
    }

    virtual std::ostream & format(std::ostream & os, LogLevel::Level level, const LogEvent::ptr & event) override  {
        return os << event->get_proc_id();
    }    
};

// %t
class ThreadIdFormatItem : public LogFormatter::FormatItem { 
public:
    using LogFormatter::FormatItem::FormatItem;
    ~ThreadIdFormatItem() {}

    virtual std::string format(LogLevel::Level level, const LogEvent::ptr & event) override {
        std::stringstream stream;
        stream << event->get_thread_id();
        return stream.str();
    }

    virtual std::ostream & format(std::ostream & os, LogLevel::Level level, const LogEvent::ptr & event) override  {
        return os << event->get_thread_id();
    }    
};

// %F
class FiberIdFormatItem : public LogFormatter::FormatItem { 
public:
    using LogFormatter::FormatItem::FormatItem;
    ~FiberIdFormatItem() {}

    virtual std::string format(LogLevel::Level level, const LogEvent::ptr & event) override {
        std::stringstream stream;
        stream << event->get_coroutine_id();
        return stream.str();
    }

    virtual std::ostream & format(std::ostream & os, LogLevel::Level level, const LogEvent::ptr & event) override  {
        return os << event->get_coroutine_id();
    }
};

// %n
class NewLineFormatItem : public LogFormatter::FormatItem { 
public:
    using LogFormatter::FormatItem::FormatItem;
    ~NewLineFormatItem() {}

    virtual std::string format(LogLevel::Level level, const LogEvent::ptr & event) override {
        return "\n";
    }

    virtual std::ostream & format(std::ostream & os, LogLevel::Level level, const LogEvent::ptr & event) override  {
        return os << std::endl;
    }
};

// %T
class TabFormatItem : public LogFormatter::FormatItem { 
public:
    using LogFormatter::FormatItem::FormatItem;
    ~TabFormatItem() {}

    virtual std::string format(LogLevel::Level level, const LogEvent::ptr & event) override {
        return "\t";
    }

    virtual std::ostream & format(std::ostream & os, LogLevel::Level level, const LogEvent::ptr & event) override  {
        return os << "\t";
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

    std::vector<std::tuple<std::string, std::string, int>> vec;
    std::string key;
    std::string value;

    // convert all char to vector type
    for (int index = 0; index < log_pattern_.size(); index++) {
        // ignore %
        if (log_pattern_[index] == '%') {
            continue;
        }

        // time format
        if (log_pattern_[index] == 'd') {
            int n = index + 1;
            assert(log_pattern_[n] == '{');
            // look for end
            while (n <= log_pattern_.size()) {
                if (log_pattern_[n] == '}') {
                    // save time format to vec
                    key = "d";
                    value = log_pattern_.substr(index+1, n-index-1);
                    // increase index to next
                    index = n + 1;
                    // add right value
                    vec.emplace_back(std::make_tuple(key, value, 0));
                    continue;
                }
                n++;
            }
            // never reach here
            assert(n <= log_pattern_.size());
        }
        vec.emplace_back(std::make_tuple(log_pattern_[index], std::string(), 0));
    }

    
}




    
}
