#include <cstdint>
#include <iostream>
#include <string>

int32_t main() {
    std::string input;
    while(true) {
        std::cout << "> ";
        if(!std::getline(std::cin, input)) break;
        if(input.empty()) continue;
        std::cout << input << '\n';
    }
    return 0;
}
