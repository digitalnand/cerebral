#pragma once

#include <array>
#include <cstdint>
#include <string>
#include <vector>

namespace crbl {

enum InstructionKind {
    INCREMENT,
    DECREMENT,
    MOVE_RIGHT,
    MOVE_LEFT,
    OUTPUT,
    READ,
    JUMP_IF_ZERO,
    JUMP_IF_NONZERO
};

struct Instruction {
    public: InstructionKind kind;
    public: int32_t value = -1; // no value

    public: std::string line;
    public: int32_t line_index;
    public: int32_t position;
};

struct Machine {
    private: std::array<int32_t, 0xfff> tape;
    private: int32_t address_pointer;
    private: std::vector<Instruction> instructions;
    // \brief Initializes machine's attributes
    public: Machine();
    // \brief Reads the target file and dumps its instructions to the machine
    public: void dump(std::ifstream&);
    // \brief Executes the internal list of instructions
    public: void execute();
};

} // end of namespace crbl
