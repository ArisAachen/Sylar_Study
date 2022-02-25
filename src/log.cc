#include "log.h"
#include "utils.h"

#include <bits/types/struct_tm.h>
#include <chrono>
#include <cstring>
#include <sstream>
#include <ctime>
#include <cassert>
#include <map>
#include <functional>
#include <stdarg.h>
#include <string>
#include <iomanip>
#include <utility>

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
    XX(ERROR);
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
    XX(ERROR);
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
    uint32_t pid, uint32_t tid, uint32_t cid, TimePoint time, const std::string & msg):
    code_file_(file), code_func_(func), code_line_(line), 
    proc_id_(pid), thread_id_(tid), coroutine_id_(cid), 
    log_time_(time), log_msg_(msg) {
    // ARIS_LOG_INFO("%v %v", num, message);
    // should check if message is valid, 
    // 1. check if count of %v is equal with full params
    

}

class StringFormatItem : public LogFormatter::FormatItem {
public:
    using LogFormatter::FormatItem::FormatItem;
    ~StringFormatItem() {}

    virtual std::string format(LogLevel::Level level, const LogEvent::ptr event) override {
        event.use_count();
        return get_item_name();
    }

    virtual std::ostream & format(std::ostream & os, LogLevel::Level level, const LogEvent::ptr event) override  {
        event.use_count();
        return os << get_item_name();
    }
};


// %f
class FilenameFormatItem : public LogFormatter::FormatItem { 
public:
    using LogFormatter::FormatItem::FormatItem;
    ~FilenameFormatItem() {}

    virtual std::string format(LogLevel::Level level, const LogEvent::ptr event) override {
        return event->get_file();
    }

    virtual std::ostream & format(std::ostream & os, LogLevel::Level level, const LogEvent::ptr event) override  {
        return os << event->get_file();
    }
};

// %l
class FileLineFormatItem : public LogFormatter::FormatItem { 
public:
    using LogFormatter::FormatItem::FormatItem;
    ~FileLineFormatItem() {}

    virtual std::string format(LogLevel::Level level, const LogEvent::ptr event) override {
        std::stringstream stream;
        stream << event->get_line();
        return stream.str();
    }

    virtual std::ostream & format(std::ostream & os, LogLevel::Level level, const LogEvent::ptr event) override  {
        return os << event->get_line();
    }    
}; 

// %m
class MessageFormatItem : public LogFormatter::FormatItem { 
public:
    using LogFormatter::FormatItem::FormatItem;
    ~MessageFormatItem() {}

    virtual std::string format(LogLevel::Level level, const LogEvent::ptr event) override {
        return event->get_log_msg();
    }

    virtual std::ostream & format(std::ostream & os, LogLevel::Level level, const LogEvent::ptr event) override  {
        return os << event->get_log_msg();
    }
};

// %p
class LogLevelFormatItem : public LogFormatter::FormatItem { 
public:
    using LogFormatter::FormatItem::FormatItem;
    ~LogLevelFormatItem() {}

    virtual std::string format(LogLevel::Level level, const LogEvent::ptr event) override {
        return LogLevel::level_to_string(level);
    }

    virtual std::ostream & format(std::ostream & os, LogLevel::Level level, const LogEvent::ptr event) override  {
        return os << LogLevel::level_to_string(level);
    }
};

// %d
class DateTimeFormatItem :  public LogFormatter::FormatItem { 
public:
    using LogFormatter::FormatItem::FormatItem;
    ~DateTimeFormatItem() {}

    virtual std::string format(LogLevel::Level level, const LogEvent::ptr event) override {
        LogEvent::TimePoint point = event->get_log_time();
        std::time_t local = std::chrono::system_clock::to_time_t(point);
        auto put = std::put_time(std::localtime(&local), item_name_.c_str());
        return std::string(put._M_fmt);
    }

    virtual std::ostream & format(std::ostream & os, LogLevel::Level level, const LogEvent::ptr event) override  {
        LogEvent::TimePoint point = event->get_log_time();
        std::time_t local = std::chrono::system_clock::to_time_t(point);
        auto put = std::put_time(std::localtime(&local), item_name_.c_str());
        return os << put;
    }
};

// %c
class ProcIdFormatItem : public LogFormatter::FormatItem { 
public:
    using LogFormatter::FormatItem::FormatItem;
    ~ProcIdFormatItem() {}

    virtual std::string format(LogLevel::Level level, const LogEvent::ptr event) override {
        std::stringstream stream;
        stream << event->get_proc_id();
        return stream.str();
    }

    virtual std::ostream & format(std::ostream & os, LogLevel::Level level, const LogEvent::ptr event) override  {
        return os << event->get_proc_id();
    }    
};

// %t
class ThreadIdFormatItem : public LogFormatter::FormatItem { 
public:
    using LogFormatter::FormatItem::FormatItem;
    ~ThreadIdFormatItem() {}

    virtual std::string format(LogLevel::Level level, const LogEvent::ptr event) override {
        std::stringstream stream;
        stream << event->get_thread_id();
        return stream.str();
    }

    virtual std::ostream & format(std::ostream & os, LogLevel::Level level, const LogEvent::ptr event) override  {
        return os << event->get_thread_id();
    }    
};

// %F
class FiberIdFormatItem : public LogFormatter::FormatItem { 
public:
    using LogFormatter::FormatItem::FormatItem;
    ~FiberIdFormatItem() {}

    virtual std::string format(LogLevel::Level level, const LogEvent::ptr event) override {
        std::stringstream stream;
        stream << event->get_coroutine_id();
        return stream.str();
    }

    virtual std::ostream & format(std::ostream & os, LogLevel::Level level, const LogEvent::ptr event) override  {
        return os << event->get_coroutine_id();
    }
};

// %n
class NewLineFormatItem : public LogFormatter::FormatItem { 
public:
    using LogFormatter::FormatItem::FormatItem;
    ~NewLineFormatItem() {}

    virtual std::string format(LogLevel::Level level, const LogEvent::ptr event) override {
        return "\n";
    }

    virtual std::ostream & format(std::ostream & os, LogLevel::Level level, const LogEvent::ptr event) override  {
        return os << std::endl;
    }
};

// %T
class TabFormatItem : public LogFormatter::FormatItem { 
public:
    using LogFormatter::FormatItem::FormatItem;
    ~TabFormatItem() {}

    virtual std::string format(LogLevel::Level level, const LogEvent::ptr event) override {
        return "\t";
    }

    virtual std::ostream & format(std::ostream & os, LogLevel::Level level, const LogEvent::ptr event) override  {
        return os << "\t";
    }    
};

LogFormatter::LogFormatter(const std::string & pattern):
    log_pattern_(pattern) {
    format_items_.clear();
    init();
}

LogFormatter::~LogFormatter() {
    format_items_.clear();
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
                    value = log_pattern_.substr(index+2, n-index-2);
                    // increase index to next
                    index = n + 1;
                    // add right value
                    vec.emplace_back(std::make_tuple(key, value, 0));
                    break;
                }
                n++;
            }
            // never reach here
            assert(n <= log_pattern_.size());
            continue;
        }
        std::stringstream ss;
        ss << log_pattern_[index];
        vec.emplace_back(std::make_tuple(ss.str(), std::string(), 0));
    }

    // create items
    static std::map<std::string, std::function<FormatItem::ptr(const std::string &)>> items = {
#define XX(str, C) \
        {#str, [](const std::string & str) { return FormatItem::ptr(new C(str)); }}

        XX(f, FilenameFormatItem), 
        XX(l, FileLineFormatItem),
        XX(m, MessageFormatItem),
        XX(p, LogLevelFormatItem),
        XX(d, DateTimeFormatItem),
        XX(c, ProcIdFormatItem),
        XX(t, ThreadIdFormatItem),
        XX(F, FiberIdFormatItem),
        XX(n, NewLineFormatItem),
        XX(T, TabFormatItem),
    };

    // find map
    for (auto it : vec) {
        auto iter = items.find(std::get<0>(it));
        // cant find a instance, regard this format as origin string
        if (iter == items.end()) {
            // add origin string "-" "[" "]"
            // format_items_.emplace_back(new StringFormatItem(std::get<0>(it)));
            format_items_.emplace_back(FormatItem::ptr(new StringFormatItem(std::get<0>(it))));
        } else {
            // emplace back DataTimeFormat("xxx")
            format_items_.emplace_back((iter->second(std::get<1>(it))));
        }
    }
}

std::string LogFormatter::format(LogLevel::Level level, const LogEvent::ptr & event) {
    std::stringstream ss;
    // append all items
    for (auto iter : format_items_) {
        ss << iter->format(level, event);
    }
    return ss.str();
}

std::ostream & LogFormatter::format(std::ostream & os, LogLevel::Level level, const LogEvent::ptr & event) {
    // append all items
    for (auto iter : format_items_) {
        iter->format(os, level, event);
    }
    return os;
}


LogLevel::Level LogAppender::get_log_level() {
    return level_;
}

void LogAppender::set_log_level(LogLevel::Level level) {
    level_ = level;
}
    
void LogAppender::set_log_format(const std::string & format) {
    formatter_.reset(new LogFormatter(format));
}

const std::string LogAppender::get_log_format() const {
    return formatter_->get_format();
}

void StdoutLogAppender::log(LogLevel::Level level, const LogEvent::ptr event) {
    Mutex::Lock lock(mutex_);
    // check if need put log to cout
    if (level_ > level) {
        return;
    }
    formatter_->format(std::cout, level, event);
}

bool FileLogAppender::init(const std::string & file) {
    file_.open(file.c_str(), std::ios_base::out | std::ios_base::app);
    return file_.is_open();
}

void FileLogAppender::log(LogLevel::Level level, const LogEvent::ptr event) {
    Mutex::Lock lock(mutex_);
    // check if need put log to file
    if (level_ > level) {
        return;
    }
    if (!file_.is_open()) {
        return;
    }
    // try to capture error
    try {
        formatter_->format(file_, level, event);
    } catch (std::exception & exp) {
        
    }
}

FileLogAppender::~FileLogAppender() {
    file_.close();
}

Logger::Logger(const std::string & name):
    name_(name) {}


Logger::~Logger() {
    appenders_.clear();
}

void Logger::addLogAppender(LogAppender::ptr appender) {
    for (auto iter : appenders_) {
        if (iter == appender)
            return;
    }
    appenders_.emplace_back(appender);
}

void Logger::delLogAppender(LogAppender::ptr appender) {
    for(auto iter = appenders_.begin(); iter != appenders_.end(); iter++) {
        if (*iter == appender) 
            continue;
        appenders_.emplace_back(appender);
        break;
    }
}

void Logger::set_name(const std::string & name) {
    name_ = name;
}

const std::string Logger::get_name() {
    return name_;
}

void Logger::log(LogLevel::Level level, const LogEvent::ptr event) {
    for (auto iter : appenders_) {
        iter->log(level, event);
    }
}

void LogMgr::addLogger(Logger::ptr logger) {
    logger_maps_.insert(std::make_pair(logger->get_name(), logger));
}

void LogMgr::log(LogLevel::Level level, const LogEvent::ptr event) {
    for (auto iter : logger_maps_) 
        iter.second->log(level, event);
}

void LogMgr::trace(const LogEvent::ptr event) {
    log(LogLevel::Level::TRACE, event);
}

void LogMgr::debug(const LogEvent::ptr event) {
    log(LogLevel::Level::TRACE, event);
}

void LogMgr::info(const LogEvent::ptr event) {
    log(LogLevel::Level::INFO, event);
}

void LogMgr::warn(const LogEvent::ptr event) {
    log(LogLevel::Level::WARN, event);
}

void LogMgr::error(const LogEvent::ptr event) {
    log(LogLevel::Level::ERROR, event);
}

void LogMgr::fatal(const LogEvent::ptr event) {
    log(LogLevel::Level::FATAL, event);
}

}
