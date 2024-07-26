#pragma once

#include <queue>
#include <string>
#include <vector>

class Job {
public:
    Job(std::string config_file);
    Job(Job&&) = default;
    Job& operator=(Job&&) = default;
private:
    std::string m_name;
    std::vector<std::string> m_tasks;
};

class Scheduler {
public:
    virtual bool queue_job(std::string config_file);
    virtual void run();
    virtual ~Scheduler() {};
};

class FIFO : public Scheduler {
public:
    FIFO() = default; 
    bool queue_job(std::string config_file) override;
    void run() override;
private:
    std::queue<Job> m_jobs;
};

