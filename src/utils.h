#ifndef __STUDY_SRC_UTILS_H__
#define __STUDY_SRC_UTILS_H__



#include "noncopable.h"
#include "thread.h"
#include <cassert>
#include <cstdarg>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <pthread.h>
#include <sstream>
#include <tuple>

namespace aris {

class StringGenerator {
public:
    static const std::string format(const std::string fmt, ...) {
        va_list ap;
        char* buf = nullptr;
        va_start(ap, fmt);
        auto len = vasprintf(&buf, fmt.c_str(), ap);
        va_end(ap);
        return std::string(buf, len);
    }
};


template<typename T>
class ScopeLockImpl {
public:
    // lock
    ScopeLockImpl(T& lock): lock_(lock) {
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