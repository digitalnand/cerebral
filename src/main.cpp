#include <cstdint>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

enum InstructionKind {
    INCREMENT,
    DECREMENT,
    MOVE_RIGHT,
    MOVE_LEFT,
    WRITE,
    READ,
    EOL,
    IGNORE
};

struct Instruction {
    InstructionKind kind;
    int8_t value;
};

struct Reader {
    std::string_view input;
    size_t index;
    Reader(const std::string_view);
    Instruction next_instruction();
    std::vector<Instruction> lex();
};

Reader::Reader(const std::string_view input) {
    this->input = input;
    this->index = 0;
}

Instruction Reader::next_instruction() {
    if(this->index >= this->input.length()) return {EOL, -1};
    switch(this->input.at(this->index++)) {
        case '+': return {INCREMENT, -1};
        case '-': return {DECREMENT, -1};
        case '>': return {MOVE_RIGHT, -1};
        case '<': return {MOVE_LEFT, -1};
        case '.': return {WRITE, -1};
        case ',': return {READ, -1};
    }
    return {IGNORE, -1};
}

std::vector<Instruction> Reader::lex() {
    std::vector<Instruction> output;
    Instruction instruction;
    while((instruction = this->next_instruction()).kind != EOL) {
        output.push_back(instruction);
    }
    return output;
}

struct Machine {
    int8_t tape[3000];
    int16_t address_pointer;
    Machine();
    void execute(const std::vector<Instruction>);
};

Machine::Machine() {
    this->address_pointer = 0;
    std::memset(this->tape, 0, sizeof this->tape);
}

void Machine::execute(const std::vector<Instruction> instructions) {
    for(const auto& instruction : instructions) {
        auto& head = this->tape[address_pointer];
        switch(instruction.kind) {
            case INCREMENT: {
                head++;
            } break;
            case DECREMENT: {
                head--;
            } break;
            case MOVE_RIGHT: {
                this->address_pointer++;
                if(this->address_pointer > static_cast<int16_t>(sizeof this->tape)) throw std::runtime_error("the cell pointer has overflowed");
            } break;
            case MOVE_LEFT: {
                this->address_pointer--;
                if(this->address_pointer < 0) throw std::runtime_error("the cell pointer has underflowed");
            } break;
            case WRITE: {
                std::putchar(head);
            } break;
            case READ: {
                const int8_t byte = std::getchar();
                std::getchar(); // consume the newline
                head = (byte != EOF) ? byte : 0;
            } break;
            case IGNORE: continue;
            default: std::unreachable();
        }
    }
}

int32_t main() {
    Machine interpreter;
    std::string input;
    do {
        if(!input.empty()) {
            Reader reader(input);
            interpreter.execute(reader.lex());
        }
        std::cout << "$ ";
    } while(std::getline(std::cin, input));
    return EXIT_SUCCESS;
}
