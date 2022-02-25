#ifndef __STUDY_SRC_FIBER_H__
#define __STUDY_SRC_FIBER_H__

#include <functional>
#include <memory>
#include <ucontext.h>
#include <atomic>

#include "noncopable.h"


namespace aris {

class Fiber : Noncopable, public std::enable_shared_from_this<Fiber> {
public:
    typedef std::shared_ptr<Fiber> ptr;
    Fiber(std::function<void()>cb , size_t stacksize = 0);
    virtual~Fiber();
    
    /**
     * @brief define fiber state
     */
    enum class State {Ready, RUNNING, TERM};
    /**
     * @brief Set the thread main fiber object
     * @param[in] fiber set main fiber
     */
    static void set_thread_main_fiber(Fiber::ptr fiber);

    /**
     * @brief Get the thread fiber count object
     */
    static int get_thread_fiber_count();

    /**
     * @brief this fiber is put in background state
     */
    void yield();

    /**
     * @brief this fiber is put int front state
     */
    void resume();

    /**
     * @brief reset fiber
     */
    void reset(std::function<void()> cb);

    /**
     * @brief 
     */
    State get_fiber_state();

    /**
     * @brief Create a main fiber object
     */
    static void create_main_fiber();

    /**
     * @brief Get the thread current fiber object
     */
    static Fiber::ptr get_thread_current_fiber();

    /**
     * @brief Get the thread main fiber object
     */
    static Fiber::ptr get_thread_main_fiber();

private:
    // create default fiber
    Fiber();

    /**
     * @brief Set the thread main fiber object
     * @param[in] fiber set this fiber as thread current fiber
     */
    static void set_thread_current_fiber(Fiber::ptr fiber);

    /**
     * @brief run fiber
     */
    static void run();

    /**
     * @brief Set the fiber state object
     * @param[in] state set current state
     */
    void set_fiber_state(State state);

private:
    /// current fiber
    uint64_t fiber_id_ {0};
    std::function<void()> cb_ {nullptr};
    State state_;

    // current context
    ucontext_t context_ {0};
    void* stack_ {nullptr};
    uint32_t stack_size_ {0};

    /// global fiber
    // static thread_local Fiber::ptr thread_main_fiber_ ;
    // static thread_local Fiber::ptr thread_current_fiber_ ;
    // static thread_local int thread_fiber_count_;
};





}



#endif