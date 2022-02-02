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
#include <pthread.h>


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
static const std::string level_to_string(Level level);

/**
 * @brief transform msg to string
 * 
 * @param msg 
 * @return Level 
 */
static const Level string_to_level(const std::string & msg);
};

class LogEvent {
public:
    /**
     * @brief delete default constructor
     * avoid creating non-param obj
     * 
     */
    LogEvent() = delete;

    // now only exist one param, other params use default value
    LogEvent(const std::string & msg);

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

class LogFormat {

};


class LogAppender {

}; 


class Logger {

};


class LogMgr {

};


};




#endif