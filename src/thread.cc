#include "thread.h"

namespace aris {

// create thread
Thread::Thread(std::function<void()> cb, std::string & name) :
    cb_(cb), name_(name) {}


// release thread
Thread::~Thread() {
    // check if thread is created
    if (thread_id_ == 0)
        return;
    // 
    int ret;
    int err = pthread_join(thread_id_, (void **)&ret);
    if (err != 0) 
        std::cout << "join thread failed, thread id: " << thread_id_ << ", err: " 
            << strerror(errno) << std::endl;

}

// run thread
void Thread::run() {
    // thread id 
    if (thread_id_ == 0) 
        return;
    int err = pthread_create(&thread_id_, nullptr, wrap, this);
    if (err != 0)
        std::cout << "create failed, thread name: " << name_ << ", err: " 
            << strerror(errno) << std::endl;    
}


void *Thread::wrap(void *arg) {
    // convert 
    auto thread =  static_cast<Thread*>(arg);
    // set thread name
    int err = pthread_setname_np(thread->thread_id_, thread->name_.c_str());
    if (err != 0) 
        std::cout << "set thread name failed, thread id: " << thread->thread_id_ << ", err: " 
            << strerror(errno) << std::endl;

    // run thread
    thread->cb_();
}

bool Thread::operator==(Thread & thread) {
    return pthread_equal(thread_id_, thread.thread_id_);
}


}