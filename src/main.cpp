#include <array>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <stack>
#include <stdexcept>
#include <string>
#include <utility>

namespace crbl {

struct Interpreter {
    std::array<int32_t, 0xfff> tape;
    int32_t address_pointer;
    Interpreter();
    void execute(const std::string_view);
};

Interpreter::Interpreter() {
    this->tape.fill(0);
    this->address_pointer = 0;
}

void display_jump_error(const std::string_view input, const int32_t position) {
    std::cerr << "\033[31m" << "error: " << "\033[0m" << "jump expected a match\n";
    std::cerr << '\t';
    for(int32_t i = 0; i < static_cast<int32_t>(input.length()); i++) {
        if(i == position) std::cerr << "\033[31m" << input.at(i) << "\033[0m";
        else              std::cerr << input.at(i);
    }
    std::cerr << "\n\t";
    for(int32_t i = 0; i < position; i++) std::cerr << ' ';
    std::cerr << "^ here\n";
}

void Interpreter::execute(const std::string_view input) {
    std::stack<int32_t> jumps;
    for(int32_t i = 0; i < static_cast<int32_t>(input.length()); i++) {
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
                if(this->address_pointer > static_cast<int32_t>(sizeof this->tape))
                    throw std::runtime_error("the cell pointer has overflowed");
            } break;
            case '<': { // move to the cell on the left
                this->address_pointer--;
                if(this->address_pointer < 0)
                    throw std::runtime_error("the cell pointer has underflowed");
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
                int32_t next_index = 0;
                const auto previous_index = i;
                for(int32_t j = i; j < static_cast<int32_t>(input.length()); j++) {
                    if(input.at(j) == '[') jumps.push(j);
                    if(input.at(j) != ']') continue;
                    jumps.pop();
                    next_index = j - 1;
                    if(jumps.empty()) break;
                }
                if(!jumps.empty()) {
                    display_jump_error(input, previous_index);
                    return;
                }
                if(head == 0) i = next_index;
            } break;
            case ']': { // jump to the matching '[' if the value of the current cell is non-zero
                int32_t next_index = 0;
                const auto previous_index = i;
                for(int32_t j = i; j >= 0; j--) {
                    if(input.at(j) == ']') jumps.push(j);
                    if(input.at(j) != '[') continue;
                    jumps.pop();
                    next_index = j - 1;
                    if(jumps.empty()) break;
                }
                if(!jumps.empty()) {
                    display_jump_error(input, previous_index);
                    return;
                }
                if(head != 0) i = next_index;
            } break;
            default: continue;
        }
    }
}

} // end of namespace crbl

int32_t main() {
    crbl::Interpreter interpreter;
    std::string input;
    do {
        if(!input.empty()) interpreter.execute(input);
        std::cout << "$ ";
    } while(std::getline(std::cin, input));
    return EXIT_SUCCESS;
}
