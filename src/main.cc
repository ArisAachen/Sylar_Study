#include "log.h"
#include "scheduler.h"

void test() {
    ARIS_LOG_FMT_INFO("test func, %s", test);
}

int main () {
    // create scheduler
    aris::Scheduler::ptr sched(new aris::Scheduler());
    for (int index = 0; index < 50; index++ ) {
        sched->schedule(test);
    }
    sched->start();
    return 1;
}