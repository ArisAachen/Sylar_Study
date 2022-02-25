#include "fiber.h"
#include "log.h"

#include <cassert>
#include <cstddef>
#include <memory>
#include <ucontext.h>

namespace aris {

class MallocStackAllocator {
public:
    static void* Alloc(size_t size) {
        return malloc(size);
    }

    static void Dealloc(void* p, size_t size) {
        return free(p);
    }
};

using StackAllocator = MallocStackAllocator;

static thread_local int thread_fiber_count_ = 0;
static thread_local Fiber::ptr thread_main_fiber_ = nullptr;
static thread_local Fiber::ptr thread_current_fiber_ = nullptr;

Fiber::Fiber(std::function<void()>cb, size_t stacksize) :
cb_(cb), stack_size_(stacksize) {
    create_main_fiber();
    if (cb == nullptr) {
        ARIS_LOG_FMT_WARN("cant create fiber with none func, create %s failed", "fiber");
        return;
    }
    // set current fiber
    fiber_id_ =  thread_fiber_count_;
    // add thread fiber
    thread_fiber_count_++;
    // malloc 
    stack_ = StackAllocator::Alloc(stacksize);
    // init state
    state_ = State::Ready;
    // get current context
    getcontext(&context_);
    context_.uc_link = nullptr;
    context_.uc_stack.ss_sp = stack_;
    context_.uc_stack.ss_size = stacksize;
    makecontext(&context_, &Fiber::run, 0);
    ARIS_LOG_FMT_INFO("create fiber success, fiber id: %d", fiber_id_);
} 

Fiber::Fiber() {
    fiber_id_ = thread_fiber_count_;
    getcontext(&context_);
    thread_fiber_count_++;
    ARIS_LOG_FMT_INFO("create default fiber success, fiber id: %d", fiber_id_);
}

void Fiber::create_main_fiber() {
    if (thread_main_fiber_ != nullptr)
        return;
    thread_main_fiber_ = Fiber::ptr(new Fiber());
    thread_current_fiber_ = thread_main_fiber_;
}

Fiber::~Fiber() {
    StackAllocator::Dealloc(stack_, stack_size_);
}

// get fiber current state
Fiber::State Fiber::get_fiber_state() {
    return state_;
}

void Fiber::set_thread_main_fiber(Fiber::ptr fiber) {
    thread_main_fiber_ = fiber;
}

Fiber::ptr Fiber::get_thread_main_fiber() {
    return thread_main_fiber_;
}

void Fiber::set_thread_current_fiber(Fiber::ptr fiber) {
    thread_current_fiber_ = fiber;
}

Fiber::ptr Fiber::get_thread_current_fiber() {
    return thread_current_fiber_;
}

// get thread fiber count
int Fiber::get_thread_fiber_count() {
    return thread_fiber_count_;
}

// yield current fiber 
void Fiber::yield() {
    // main fiber cant be yield, 
    // only children fiber can yield, so main fiber put in font ground
    try {
        if (fiber_id_ == thread_main_fiber_->fiber_id_)
            throw std::logic_error("main fiber cannot be yield");

        // yield current fiber, exec main fiber
        set_thread_current_fiber(thread_main_fiber_);
        thread_main_fiber_->set_fiber_state(State::RUNNING);
        // swap current fiber to main fiber
        state_ = State::Ready;
        swapcontext(&context_, &thread_main_fiber_->context_);
    } catch (std::exception& e) {
        ARIS_LOG_FMT_WARN("yield failed, err: %s",e.what());
        return;
    }
    ARIS_LOG_FMT_INFO("fiber yield successfully, fiber id: %d", fiber_id_);
}

// resume execute current fiber
void Fiber::resume() {
    // current fiber is already in execute 
    try {
        // in case cant get back to main fiber
        // other fiber must checkout from main fiber
        // main fiber cant resume, because main fiber func has nothing to do 
        if (thread_main_fiber_->fiber_id_ == fiber_id_) 
            throw std::logic_error("main fiber cannot resume");
        if (thread_current_fiber_ != thread_main_fiber_) 
            throw std::logic_error("current fiber is not main fiber");
        if (thread_current_fiber_->fiber_id_ == fiber_id_) 
            throw std::logic_error("fiber id is the same");
        // set current fiber as now
        set_thread_current_fiber(shared_from_this());
        state_ = State::RUNNING;
        // swap to current fiber
        thread_main_fiber_->set_fiber_state(State::Ready);
        swapcontext(&thread_main_fiber_->context_, &context_);
    } catch (std::exception& e) {
        ARIS_LOG_FMT_WARN("fiber resume failed, fiber id: %d, err: %s", fiber_id_, e.what());
        return;
    }
    // ARIS_LOG_FMT_INFO("fiber resume successfully, fiber id: %d", fiber_id_);
}

// set fiber state
void Fiber::set_fiber_state(Fiber::State state) {
    state_ = state;
}

// reset fiber, reuse fiber
void Fiber::reset(std::function<void()> cb) {
    try {
        if (thread_main_fiber_ && fiber_id_ == thread_main_fiber_->fiber_id_)
            throw std::logic_error("main fiber cannot be reset");
        cb_.swap(cb);
        // yield
        yield();
    } catch (std::exception& e) {
        ARIS_LOG_FMT_WARN("fiber reset failed, fiber id: %d, err: %s", fiber_id_, e.what());
        return;
    }
    // normally this log will not output
    ARIS_LOG_FMT_INFO("fiber reset successfully, fiber id: %d", fiber_id_);
}

// try to run func
void Fiber::run() {
    try {
        auto fiber = thread_current_fiber_;
        // main fiber has no func, can not run
        if (fiber == thread_main_fiber_) 
            throw std::logic_error("main fiber cannot run");
        // run fiber func    
        if (fiber->cb_)
            fiber->cb_();
        // set fiber as term    
        fiber->set_fiber_state(State::TERM);
        // reset fiber
        fiber->yield();
    } catch (std::exception& e) {
        ARIS_LOG_FMT_WARN("fiber run failed, err: %s",e.what());
        return;
    }
}

}