#include "log.h"
#include "scheduler.h"
#include "singelton.h"

void test() {
    ARIS_LOG_FMT_INFO("test func, %s", "test");
}

int main () {
    auto logger = aris::Logger::ptr(new aris::Logger());
    logger->addLogAppender(aris::LogAppender::ptr(new aris::StdoutLogAppender()));
    aris::SingeltonPtr<aris::LogMgr>::get_instance()->addLogger(logger);

    // create scheduler
    aris::Scheduler::ptr sched(new aris::Scheduler());
    for (int index = 0; index < 1; index++ ) {
        sched->schedule(test);
    }
    sched->start();
    return 1;
}