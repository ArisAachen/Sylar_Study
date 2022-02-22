#ifndef __STUDY_SRC_SCHEDULER_H__
#define __STUDY_SRC_SCHEDULER_H__

#include "fiber.h"
#include "noncopable.h"
#include "thread.h"
#include "utils.h"

#include <functional>
#include <memory>
#include <pthread.h>
#include <string>
#include <vector>
#include <queue>

namespace aris {

class Scheduler : Noncopable {
public:
    typedef Mutex MutexType;
    typedef std::shared_ptr<Scheduler> ptr;

    /**
     * @brief Construct a new Scheduler object
     * 
     * @param[in] thread_count create thread count
     * @param[in] name scheduler name
     */
    Scheduler(int thread_count = 16, const std::string & name = "scheduler");

    virtual~Scheduler();

    /**
     * @brief 
     * @param[in] cb exec func, should regard as fiber task
     */
    void schedule(std::function<void()> cb);

    /**
     * @brief start all thread to exec
     */
    void start();

private:
    struct ScheduleTask {
        typedef std::shared_ptr<ScheduleTask> ptr;
        ScheduleTask() {

        }
        /**
         * @brief Construct a new Schedule Task object
         * @param[in] fiber fiber task
         * @param[in] thread execute thread 
         */
        ScheduleTask(Fiber::ptr fi, int thr = -1) {
            fiber = fi;
            thread = thr;
        }
        /**
         * @brief Construct a new Schedule Task object
         * @param[in] cb fiber func
         * @param[in] thread exec thread
         */
        ScheduleTask(std::function<void()> cb, int thr = -1) {
            fiber->reset(cb);
            thread = thr;
        }
        /**
         * @brief reset task
         */
        void reset() {
            fiber = nullptr;
        }

        /**
         * @brief execute fiber
         */
        void run() {
            // check if fiber eixst
            if (!fiber)
                return;
            fiber->resume();
        }

        /// fiber task
        Fiber::ptr fiber {nullptr};
        /// add task to which 
        int thread {-1};
    };

private:
    /**
     * @brief run scheduler
     */
    void run();

    /**
     * @brief idle 
     */
    void idle();

private:
    /// state
    bool stop_ {false};
    pthread_cond_t cont_ ;
    
    /// idle fiber
    Fiber::ptr idle_fiber_ {new Fiber(std::bind(&Scheduler::idle, this))};

    // thread
    std::string name_ {""};
    int thread_count_ {0};
    std::vector<Thread::ptr> threads_ {};

    /// task queu
    MutexType mutex_ ;
    std::queue<ScheduleTask::ptr> tasks_ {};
};


}

#endif