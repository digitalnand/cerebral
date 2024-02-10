#include <cstdint>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <stack>
#include <stdexcept>
#include <string>
#include <utility>

struct Machine {
    int8_t tape[3000];
    int16_t address_pointer;
    Machine();
    void execute(const std::string_view);
};

Machine::Machine() {
    this->address_pointer = 0;
    std::memset(this->tape, 0, sizeof this->tape);
}

void Machine::execute(const std::string_view input) {
    std::stack<int16_t> jumps;
    for(int16_t i = 0; i < static_cast<int16_t>(input.length()); i++) {
        auto& head = this->tape[address_pointer];
        switch(input.at(i)) {
            case '+': { // increment the current cell
                head++;
            } break;
            case '-': { // decrement the current cell
                head--;
            } break;
            case '>': { // move to the cell on the right
                this->address_pointer++;
                if(this->address_pointer > static_cast<int16_t>(sizeof this->tape)) throw std::runtime_error("the cell pointer has overflowed");
            } break;
            case '<': { // move to the cell on the left
                this->address_pointer--;
                if(this->address_pointer < 0) throw std::runtime_error("the cell pointer has underflowed");
            } break;
            case '.': { // output the value of the current cell
                std::putchar(head);
            } break;
            case ',': { // replace the value of the current cell with the first byte of the user input
                const int8_t byte = std::getchar();
                std::getchar(); // consume the newline
                head = (byte != EOF) ? byte : 0;
            } break;
            case '[': { // jump to the matching ']' if the value of the current cell is zero
                if(head != 0) continue;
                for(int16_t j = i; j < static_cast<int16_t>(input.length()); j++) {
                    if(input.at(j) == '[') jumps.push(j);
                    if(input.at(j) != ']') continue;
                    jumps.pop();
                    i = j - 1;
                    if(jumps.empty()) break;
                }
                if(!jumps.empty()) throw std::runtime_error("unbalanced jump");
            } break;
            case ']': { // jump to the matching '[' if the value of the current cell is non-zero
                if(head == 0) continue;
                for(int16_t j = i; j >= 0; j--) {
                    if(input.at(j) == ']') jumps.push(j);
                    if(input.at(j) != '[') continue;
                    jumps.pop();
                    i = j - 1;
                    if(jumps.empty()) break;
                }
                if(!jumps.empty()) throw std::runtime_error("unbalanced jump");
            } break;
            default: continue;
        }
    }
}

int32_t main() {
    Machine interpreter;
    std::string input;
    do {
        if(!input.empty()) interpreter.execute(input);
        std::cout << "$ ";
    } while(std::getline(std::cin, input));
    return EXIT_SUCCESS;
}
