#pragma once

#include <ctime>
#include <list>
#include <string>
#include <thread>
#include <vector>
#include <mutex>
#include <rpc/this_handler.h>


struct Task {
    std::time_t started;
    std::string filename;
    int         task_number;
    bool        isMap;
    MSGPACK_DEFINE_ARRAY(started, filename, task_number, isMap)
};

struct Done {
    MSGPACK_DEFINE_ARRAY()
};

class Coordinator {

public:
    Coordinator(const std::vector<std::string>& filenames);
    Coordinator(const Coordinator&) = delete;
    Coordinator& operator=(const Coordinator&) = delete;

    void start();
    bool done() const noexcept;

private:
    std::list<Task>     m_queued;
    std::list<Task>     m_in_progress;
    std::mutex          m_mu;
    int                 m_reduce_task_count;
    bool                m_finished_map;
    bool                m_done;

private:
    bool is_expired() const noexcept;
    Task get_task();
    Done retire_task(int task);
    void check_if_done();
    void queue_reduce_tasks();

};
