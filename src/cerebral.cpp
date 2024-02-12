#include <fstream>
#include <iostream>
#include <stack>

#include "cerebral.h"

namespace crbl {

void display_error(const std::string_view line, const int32_t line_index, const int32_t position, const std::string_view message) {
    std::cerr << "\033[31m" << "error: " << "\033[0m" << message << '\n';
    std::cerr << " " << line_index << " |";
    std::cerr << '\t';
    for(int32_t i = 0; i < static_cast<int32_t>(line.length()); i++) {
        if(i == position) std::cerr << "\033[31m" << line.at(i) << "\033[0m";
        else              std::cerr << line.at(i);
    }
    std::cerr << "\n \t";
    for(int32_t i = 0; i < position; i++) std::cerr << ' ';
    std::cerr << "^ here\n";
}

Machine::Machine() {
    this->tape.fill(0);
    this->address_pointer = 0;
}

void Machine::dump(std::ifstream& file) {
    std::stack<int32_t> jumps;
    int32_t instruction_index = 0;
    int32_t line_index = 0;
    std::string line;

    while(std::getline(file, line)) {
        line_index++;
        for(int32_t i = 0; i < static_cast<int32_t>(line.length()); i++) {
            const auto& character = line.at(i);

            Instruction current;
            current.line = line;
            current.line_index = line_index;
            current.position = i;

            switch(character) {
                case '+': {
                    current.kind = INCREMENT;
                    current.value = 1;
                } break;
                case '-': {
                    current.kind = DECREMENT;
                    current.value = 1;
                } break;
                case '>': {
                    current.kind = MOVE_RIGHT;
                    current.value = 1;
                } break;
                case '<': {
                    current.kind = MOVE_LEFT;
                    current.value = 1;
                } break;
                case '.': {
                    current.kind = OUTPUT;
                } break;
                case ',': {
                    current.kind = READ;
                } break;
                case '[': {
                    jumps.push(instruction_index);
                    current.kind = JUMP_IF_ZERO;
                } break;
                case ']': {
                    if(jumps.empty()) {
                        display_error(line, line_index, i, "jump expected a match");
                        exit(EXIT_FAILURE);
                    }
                    const auto index = jumps.top(); jumps.pop();
                    current.kind = JUMP_IF_NONZERO;
                    current.value = index;
                    this->instructions[index].value = instruction_index;
                } break;
                default: continue;
            }
            this->instructions.push_back(current);
            instruction_index++;
        }
    }

     if(!jumps.empty()) {
        const auto index = jumps.top(); jumps.pop();
        const auto& instruction = this->instructions.at(index);
        display_error(instruction.line, instruction.line_index, instruction.position, "jump expected a match");
        exit(EXIT_FAILURE);
    }
}

void Machine::execute() {
    for(int32_t i = 0; i < static_cast<int32_t>(this->instructions.size()); i++) {
        const auto& instruction = instructions.at(i);
        auto& head = this->tape[address_pointer];
        switch(instruction.kind) {
            case INCREMENT: {
                head += instruction.value;
            } break;
            case DECREMENT: {
                head -= instruction.value;
            } break;
            case MOVE_RIGHT: {
                this->address_pointer += instruction.value;
                if(this->address_pointer > static_cast<int32_t>(sizeof this->tape)) {
                    display_error(instruction.line, instruction.line_index, instruction.position, "cell pointer has overflowed");
                    exit(EXIT_FAILURE);
                }
            } break;
            case MOVE_LEFT: {
                this->address_pointer -= instruction.value;
                if(this->address_pointer < 0) {
                    display_error(instruction.line, instruction.line_index, instruction.position, "cell pointer has underflowed");
                    exit(EXIT_FAILURE);
                }
            } break;
            case OUTPUT: {
                std::putchar(head);
            } break;
            case READ: {
                const int8_t byte = std::getchar();
                std::getchar(); // consume the newline
                head = (byte != EOF) ? byte : 0;
            } break;
            case JUMP_IF_ZERO: {
                if(head != 0) continue;
                i = instruction.value;
            } break;
             case JUMP_IF_NONZERO: {
                if(head == 0) continue;
                i = instruction.value;
            } break;
        }
    }
}

} // end of namespace crbl
