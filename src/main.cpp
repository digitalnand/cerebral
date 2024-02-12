#include <format>
#include <fstream>
#include <iostream>

#include "cerebral.hpp"

int32_t main(int32_t argc, char* argv[]) {
    if(argc < 2) {
        std::cout << "Usage: " << argv[0] << " [FILE]\n";
        return EXIT_FAILURE;
    }

    const auto file_path = std::string(argv[1]);
    std::ifstream file(file_path);
    if(!file.is_open()) {
        throw std::runtime_error(std::format("could not open file: {}", file_path));
    }

    crbl::Machine machine;
    machine.dump(file);
    machine.execute();

    return EXIT_SUCCESS;
}
