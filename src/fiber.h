#ifndef __STUDY_SRC_FIBER_H__
#define __STUDY_SRC_FIBER_H__

namespace aris {

#include <functional>
#include <memory>


class Fiber {
public:
    typedef std::shared_ptr<Fiber> ptr;
    Fiber(std::function<void()>cb, size_t stacksize);
    
};










}



#endif