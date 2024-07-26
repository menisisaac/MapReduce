#include <cstdio>
#include <rpc/client.h>
#include <iostream>
#include "../include/Coordinator.h"
#include "../include/Plugin.h"
#include "nlohmann/json_fwd.hpp"
#include <fstream>
#include <sstream>
#include <nlohmann/json.hpp>
#include <thread>


inline int n_reduce(const std::string& key, int nreduce) {
    std::hash<std::string> hasher;
    return hasher(key) % nreduce;
}

void doTask(Task& task, int nreduce) {
    std::ifstream f(task.filename); 
    std::string str;
    if(f) {
        std::ostringstream ss;
        ss << f.rdbuf();
        str = ss.str();
        auto result = Map(str);
        std::vector<std::vector<std::pair<std::string, std::string>>> files(nreduce);
        for(auto& line : result) {
            files[n_reduce(line.first, nreduce)].push_back(line);
        }
        std::vector<std::pair<std::string, std::fstream>> filenames(nreduce);
        for(auto& filename : filenames) {
            filename.first = std::tmpnam(nullptr);
            filename.second = std::fstream(filename.first);
            filename.second.open(filename.first, std::fstream::out);
        }
        for(int i = 0; i < nreduce; i++) {
            nlohmann::json j_vec(files[i]);
            filenames[i].second << j_vec.dump();
            filenames[i].second.close();
            std::rename(filenames[i].first.c_str(), "newfile.txt");
        }
    } else {
        return;
    }

}

void start() {
    rpc::client client("127.0.0.1", rpc::constants::DEFAULT_PORT);
    while(true) {
        auto result = client.call("RequestTask").as<Task>();
        if(result.filename != "") {
            doTask(result, 10);
            client.call("RetireTask", result.task_number).as<Done>();
        } else {
            {
                using namespace std::chrono_literals;
                std::this_thread::sleep_for(2000ms);
            }
        }
        std::cout << "Test" << result.task_number;
        if(result.task_number == -2)
            break;
    }
}

int main() {
    start();
}
