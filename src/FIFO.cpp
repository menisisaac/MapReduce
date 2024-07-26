#include "../include/Scheduler.h"
#include <rpc/server.h>

bool FIFO::queue_job(std::string task) {
    m_jobs.push(Job(task));
    return true;
}

void FIFO::run() {
    rpc::server scheduler("127.0.0.1", 8080);
    scheduler.bind("QueueJob", [&](std::string task) { return this->queue_job(task); });
    scheduler.suppress_exceptions(true);
    scheduler.async_run();
}
