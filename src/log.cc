#include "log.h"


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
    case LogLevel::name: \
        return #name;

    XX(TRACE);
    XX(DEBUG;
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
const Level LogLevel::string_to_level(const std::string & msg) {
    switch (msg) {
#define XX(name) \
    case #name:  \
        return Level::name;

    XX(TRACE);
    XX(DEBUG;
    XX(INFO);
    XX(WARN);
    XX(FATAL);
    default:
        break;
    }
    return Level::UNKOWN;
}

LogEvent::LogEvent(const std::string & msg):



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

    
}
