#include <cstdint>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>

struct Machine {
    int8_t tape[3000];
    int16_t address_pointer;
    Machine();
    void eval(const std::string_view);
};

Machine::Machine() {
    this->address_pointer = 0;
    std::memset(this->tape, 0, sizeof this->tape);
}

void Machine::eval(const std::string_view input) {
    for(const auto& character : input) {
        auto& head = this->tape[address_pointer];
        switch(character) {
            case '+': {
                head++;
            } break;
            case '-': {
                head--;
            } break;
            case '>': {
                this->address_pointer++;
                if(this->address_pointer > static_cast<int16_t>(sizeof this->tape)) throw std::runtime_error("the cell pointer has overflowed");
            } break;
            case '<': {
                this->address_pointer--;
                if(this->address_pointer < 0) throw std::runtime_error("the cell pointer has underflowed");
            } break;
            case '.': {
                std::putchar(head); std::putchar('\n');
            } break;
            case ',': {
                const int8_t byte = std::getchar();
                std::getchar(); // consume the newline
                head = (byte != EOF) ? byte : 0;
            } break;
            default: continue;
        }
    }
}

int32_t main() {
    Machine interpreter;
    std::string input;
    do {
        if(!input.empty()) interpreter.eval(input);
        std::cout << "$ ";
    } while(std::getline(std::cin, input));
    return EXIT_SUCCESS;
}
