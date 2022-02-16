#include "fiber.h"
#include "log.h"

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

Fiber::Fiber(std::function<void()>cb, size_t stacksize) :
cb_(cb), stack_size_(stacksize) {
    // add thread fiber
    thread_fiber_count_++;
    // set current fiber
    fiber_id_ =  thread_fiber_count_;
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

Fiber::~Fiber() {
    
}

void Fiber::set_thread_main_fiber(Fiber::ptr fiber) {
    thread_main_fiber_ = fiber;
}

Fiber::ptr Fiber::get_thread_main_fiber() {
    return thread_main_fiber_->shared_from_this();
}

void Fiber::set_thread_current_fiber(Fiber::ptr fiber) {
    thread_current_fiber_ = fiber;
}

Fiber::ptr Fiber::get_thread_current_fiber() {
    return thread_current_fiber_->shared_from_this();
}


}