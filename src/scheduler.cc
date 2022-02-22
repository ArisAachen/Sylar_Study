#include "scheduler.h"
#include "fiber.h"
#include "log.h"
#include "thread.h"
#include "utils.h"
#include "macro.h"

#include <functional>
#include <algorithm>
#include <pthread.h>
#include <queue>
#include <string>
#include <unistd.h>

namespace aris {

Scheduler::Scheduler(int thread_count, const std::string & name) {
    pthread_cond_init(&cont_, nullptr);
    // save name
    name_ = name;
    // create thread
    for (int index; index < thread_count; index++) {
        // append create thread
        threads_.emplace_back(Thread::ptr(new Thread(std::bind(&Scheduler::run, this), 
            "scheduler_thread+" + std::to_string(index))));
    }
}

Scheduler::~Scheduler() {
    pthread_cond_destroy(&cont_);
    threads_.clear();
    std::queue<ScheduleTask::ptr> tmp;
    tasks_.swap(tmp);
    stop_ = true;
}

// schedule
void Scheduler::schedule(std::function<void ()> cb) {
    // push task
    tasks_.push(ScheduleTask::ptr(new ScheduleTask(cb)));
    pthread_cond_signal(&cont_);
}

// 
void Scheduler::start() {
    stop_ = false;
    // start to run all thread
    for_each(threads_.cbegin(), threads_.cend(), [](Thread::ptr thread) {
        thread->run();
    });
    // this thread should also added to fiber
    run();
}

void Scheduler::run() {
    //  get front elem
    ScheduleTask::ptr task;
    {
        MutexType::Lock lock(mutex_);
        task = tasks_.front();
        tasks_.pop();
    }

    // check if task is in ready state
    ARIS_ASSERT(task->fiber->get_fiber_state() == Fiber::State::Ready);
    // run task
    task->run();
    // if tasks is now empty, should idle here
    if (tasks_.empty()) {
        if (stop_) {
            ARIS_LOG_FMT_INFO("schedule stop, should end, scheduler name: %s", name_);
            return;
        }
        idle_fiber_->resume();
    }
}

void Scheduler::idle() {
    if (stop_) 
        return;
    ARIS_LOG_FMT_DEBUG("begin to idle, %s", "sleeping");
    // it
    pthread_cond_wait(&cont_, nullptr);
}

}