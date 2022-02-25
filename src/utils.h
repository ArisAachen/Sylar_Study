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
class ScopedLockImpl {
public:
    // lock
    ScopedLockImpl(T& lock): lock_(lock) {
        lock_.lock();
        is_locked_ = true;
    }
    // unlock
    virtual~ScopedLockImpl() {
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
    typedef ScopedLockImpl<Mutex> Lock;
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



template <typename T>
class ScopedCondImpl {
public:
    ScopedCondImpl(T& cond): cond_(cond) {
        lock();
    }

    // wait
    void wait() {
        lock();
        cond_.wait();
        unlock();
    }

    // signal
    void signal() {
        unlock();
        cond_.signal();
        locked_ = false;
    }

    // lock
    void lock() {
        if (locked_) 
            return;
        locked_ = true;
        cond_.lock();
    }

    // unlock
    void unlock() {
        if (!locked_) 
            return;
        locked_ = false;
        cond_.unlock();
    }
    
    ~ScopedCondImpl() {
        signal();
    }

private:
    bool locked_ {false};
    T& cond_;
};


class Cond {
public:
    typedef ScopedCondImpl<Cond> Wait;
    Cond() {
        pthread_cond_init(&cond_, nullptr);
        pthread_mutex_init(&mutex_, nullptr);
    }

    // wait
    void wait() {
        pthread_cond_wait(&cond_, &mutex_);
    }

    // signal
    void signal() {
        pthread_cond_signal(&cond_);
    }

    // broadcast
    void broadcast() {
        pthread_cond_broadcast(&cond_);
    }

    // lock
    void lock() {
        pthread_mutex_lock(&mutex_);
    }

    // unlock
    void unlock() {
        pthread_mutex_unlock(&mutex_);
    }

    ~Cond() {
        pthread_mutex_destroy(&mutex_);
        pthread_cond_destroy(&cond_);
    }

private:
    pthread_mutex_t mutex_;
    pthread_cond_t cond_;
};


}


#endif