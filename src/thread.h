#ifndef __STUDY_SRC_THREAD_H__
#define __STUDY_SRC_THREAD_H__

namespace aris {

#include <pthread.h>
#include <functional>


class Thread {
public:
    /**
     * @brief 
     * 
     * @param[in] cb thread func 
     * @param[in] name thread name
     */
    Thread(std::function<void*(void*)> cb, std::string & name);
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
     * @brief get thread id
     * 
     * @return * pthread_t 
     */
    pthread_t get_thread_id();
    // check if thread is the same
    bool operator==(Thread & thread);

private:
    /// thread exec func
    std::function<void*(void*)> cb_ {nullptr};
    /// thread name
    std::string name_ {""};
    /// thread id
    pthread_t thread_id_ {0};
    /// proc id
    pid_t proc_id {0};
};




}



#endif