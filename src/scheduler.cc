#include "scheduler.h"
#include "log.h"
#include "thread.h"
#include "utils.h"

#include <functional>
#include <algorithm>

namespace aris {

Scheduler::Scheduler(int thread_count, const std::string & name) {
    // save name
    name_ = name;
    // create thread
    for (int index; index < thread_count; index++) {
        threads_.emplace_back(Thread::ptr(new Thread(std::bind(&Scheduler::run, this), "scheduler")));
    }
}

// 
void Scheduler::start() {
    // start to run all thread
    for_each(threads_.cbegin(), threads_.cend(), [](Thread::ptr thread) {
        thread->run();
    });
}



void Scheduler::run() {
    
}


}