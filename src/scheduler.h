#ifndef __STUDY_SRC_SCHEDULER_H__
#define __STUDY_SRC_SCHEDULER_H__

#include "fiber.h"
#include "noncopable.h"
#include "thread.h"
#include "utils.h"

#include <functional>
#include <memory>
#include <pthread.h>
#include <queue>
#include <string>
#include <vector>

namespace aris {

class Scheduler : Noncopable {
public:
    typedef Cond CondType;
    typedef std::shared_ptr<Scheduler> ptr;

    /**
     * @brief Construct a new Scheduler object
     * 
     * @param[in] thread_count create thread count
     * @param[in] name scheduler name
     */
    Scheduler(int thread_count = 1, const std::string & name = "scheduler");

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
    struct ScheduleTask : std::enable_shared_from_this<ScheduleTask>{
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
        ScheduleTask(std::function<void()> cb, int thr = -1):
        fiber(new Fiber(cb)) {            
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
    
    /// idle fiber
    Fiber::ptr idle_fiber_ {new Fiber(std::bind(&Scheduler::idle, this))};

    // thread
    std::string name_ {""};
    int thread_count_ {0};
    std::vector<Thread::ptr> threads_ {};

    /// task queu
    CondType cond_;
    std::queue<ScheduleTask::ptr> tasks_ {};
};


}

#endif