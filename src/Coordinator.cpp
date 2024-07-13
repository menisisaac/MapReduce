#include <rpc/server.h>
#include "../include/Coordinator.h"
#include <mutex>
#include <chrono>


Coordinator::Coordinator(const std::vector<std::string>& filenames) {
    m_reduce_task_count = 10;
    m_finished_map = false;
    m_done = false;
    int i = 0;
    time_t temp;
    for(const auto& filename : filenames) {
        m_queued.push_back({temp, filename, i++, true});
    }
}

bool Coordinator::is_expired() const noexcept {
    std::time_t current = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::time_t started = !m_in_progress.empty()?m_in_progress.front().started:current;
    if(current - started > 10) {
        return true;
    }
    return false;
}

bool Coordinator::done() const noexcept { return m_done; };

void Coordinator::retire_task(int task_target) { 
    std::scoped_lock guard(m_mu);
    bool exists;
    for(auto& task : m_queued) {
        
    }
    m_in_progress.remove_if([=](Task& task) { return task.task_number == task_target; }); 
}

void Coordinator::queue_reduce_tasks() {
    for(int i = 0; i < 10; i++) {
        m_queued.push_back({0, "", i, false});
    }
    m_finished_map = true;
}

void Coordinator::check_if_done() {
    std::scoped_lock guard(m_mu);
    if(m_queued.empty() && m_in_progress.empty() && m_finished_map)
        m_done = true;
}

Task Coordinator::get_task() {
    std::scoped_lock guard(m_mu);
    if(m_done)
        return {0, "", -2, false};
    if(!m_queued.empty()) {
        Task task = m_queued.front(); 
        m_queued.pop_front();
        task.started = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        m_in_progress.push_back(std::move(task));
        return m_in_progress.back();
    } else {
        if(is_expired()) {
            m_in_progress.push_back(m_in_progress.front());
            m_in_progress.pop_front();
            m_in_progress.back().started = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            return m_in_progress.back();
        }
    }
    return {0, "", -1, false};

}

void Coordinator::start() {
    rpc::server srv("127.0.0.1", rpc::constants::DEFAULT_PORT);
    srv.bind("RequestTask", [&]() { return this->get_task(); });
    srv.bind("RetireTask", [&](int task_target) { this->retire_task(task_target); });
    srv.suppress_exceptions(true);
    srv.run();
}
