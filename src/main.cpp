#include "../include/Coordinator.h"
#include <vector>

int main(int argc, char** argv) {
    std::vector<std::string> filenames = {"test.txt"};
    Coordinator coord(filenames);
    coord.start();
}
