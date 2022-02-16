#ifndef __STUDY_SRC_FIBER_H__
#define __STUDY_SRC_FIBER_H__

namespace aris {

#include <functional>
#include <memory>
#include <ucontext.h>

#include "noncopable.h"


namespace aris {

class Fiber : Noncopable, public std::enable_shared_from_this<Fiber> {
public:
    typedef std::shared_ptr<Fiber> ptr;
    Fiber(std::function<void()>cb, size_t stacksize);
    virtual~Fiber();

    /**
     * @brief Set the thread main fiber object
     * 
     * @param[in] fiber set main fiber
     */
    static void set_thread_main_fiber(Fiber::ptr fiber);

private:
    /**
     * @brief Set the thread main fiber object
     * 
     * @param[in] fiber set this fiber as thread current fiber
     */
    static void set_thread_current_fiber(Fiber::ptr fiber);


private:
    /// current fiber
    uint64_t fiber_id_ {0};
    ucontext_t context_ {0};
    void* stack_ {nullptr};
    uint32_t stack_size_ {0};

    /// global fiber
    static thread_local Fiber::ptr thread_main_fiber_;
    static thread_local std::atomic<int> thread_fiber_count_;
};





}




}



#endif