#ifndef __STUDY_SRC_SCHEDULER_H__
#define __STUDY_SRC_SCHEDULER_H__

#include "fiber.h"
#include "noncopable.h"
#include "thread.h"
#include "utils.h"

#include <memory>
#include <string>
#include <vector>
#include <queue>


namespace aris {

class Scheduler : Noncopable {
public:
    typedef std::shared_ptr<Scheduler> ptr;

    /**
     * @brief Construct a new Scheduler object
     * 
     * @param[in] thread_count create thread count
     * @param[in] name scheduler name
     */
    Scheduler(int thread_count = 16, const std::string & name = "scheduler");

    /**
     * @brief ru scheduler
     */
    void run();

private:
    std::string name_ {""};
    std::vector<Thread::ptr> threads_ {};

    // task queu
    Mutex::Lock task_lock_;
    std::queue<Fiber::ptr> task {};
};


}

#endif