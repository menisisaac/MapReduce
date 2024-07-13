#include "../include/Plugin.h"

std::vector<std::pair<std::string, std::string>> Map(std::string str) {
    std::vector<std::pair<std::string, std::string>> result;

    std::string curr = "";
    for(auto& ch : str) {
        if(isalpha(ch))
            curr += ch;
        else {
            if(curr != "")
                result.push_back({curr, "1"});
            curr = "";
        }
    }
    if(curr != "")
        result.push_back({curr, "1"});
    return result;
}
