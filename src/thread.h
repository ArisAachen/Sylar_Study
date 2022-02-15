#ifndef __STUDY_SRC_THREAD_H__
#define __STUDY_SRC_THREAD_H__

namespace aris {

#include <pthread.h>
#include <functional>
#include <iostream>
#include <cstring>

class Thread {
public:
    /**
     * @brief 
     * 
     * @param[in] cb thread func 
     * @param[in] name thread name
     */
    Thread(std::function<void()> cb, std::string & name);
    /**
     * @brief release thread
     * 
     */
    virtual~Thread();

public:    
    /**
     * @brief run thread
     * 
     */
    void run();
    /**
     * @brief wrap 
     * 
     * @param[in] arg thread arg
     */
    static void *wrap(void* arg);
    /**
     * @brief get thread id
     */
    pthread_t get_thread_id();

    /**
     * @brief get thread name
     */
    const std::string get_thread_name();

    /**
     * @brief check if thread id is equal
     */
    bool operator==(Thread & thread);

private:
    /// thread exec func
    std::function<void()> cb_ {nullptr};
    /// thread name
    std::string name_ {""};
    /// thread id
    pthread_t thread_id_ {0};
    /// proc id
    pid_t proc_id {0};
};




}



#endif