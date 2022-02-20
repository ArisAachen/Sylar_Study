#ifndef __STUDY_SRC_UTILS_H__
#define __STUDY_SRC_UTILS_H__



#include "noncopable.h"
#include <cassert>
#include <string>
#include <pthread.h>

namespace aris {

class StringGenerator {
public:
    template<typename... Args>
    static const std::string format(const std::string & fmt, Args... args) {
        char buf[128];
        snprintf(buf, 128, fmt.c_str(), args...);
        return std::string(buf);
    }
};


template<typename T>
class ScopeLockImpl {
public:
    // lock
    ScopeLockImpl(T& lock) {
        lock_ = lock;
        lock_.lock();
        is_locked_ = true;
    }
    // unlock
    virtual~ScopeLockImpl() {
        unlock();
    }

    void lock() {
        if (is_locked_)
            return;
        lock_.lock();
        is_locked_ = true;
    }

    void unlock() {
        if (!is_locked_)
            return;
        lock_.unlock();
        is_locked_ = false; 
    }

private:
    bool is_locked_ {false};
    T& lock_;
};


class Mutex {
public:
    typedef ScopeLockImpl<Mutex> Lock;
    Mutex() {
        pthread_mutex_init(&lock_, nullptr);
    }
    ~Mutex() {
        pthread_mutex_destroy(&lock_);
    }
    void lock() {
        pthread_mutex_lock(&lock_);
    }
    void unlock() {
        pthread_mutex_unlock(&lock_);
    }
private:
    pthread_mutex_t lock_;
};




}


#endif