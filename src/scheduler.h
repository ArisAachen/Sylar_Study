#ifndef __STUDY_SRC_SCHEDULER_H__
#define __STUDY_SRC_SCHEDULER_H__

#include "noncopable.h"

#include <memory>


namespace aris {

class Scheduler : Noncopable {
public:
    typedef std::shared_ptr<Scheduler> ptr;

    Scheduler();

};


}

#endif