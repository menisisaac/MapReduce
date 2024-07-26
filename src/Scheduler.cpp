#include "../include/Scheduler.h"
#include <yaml-cpp/yaml.h>

Job::Job(std::string config_file) {
    auto config = YAML::LoadFile(config_file);
    auto name   = config["name"].as<std::string>();
    m_name = name;
    auto tasks  = config["tasks"];
    for(int i = 0; i < tasks.size(); i++) {
        m_tasks.push_back(tasks[i].as<std::string>());
    }
}





