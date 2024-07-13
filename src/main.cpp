#include "../include/Coordinator.h"
#include <vector>

int main() {
    std::vector<std::string> filenames = {"test.txt"};
    Coordinator coord(filenames);
    coord.start();
}
