#include "chip.hpp"
#include <fstream>

#include <iostream>
using namespace std;

#define START_ADRESS 0x200

Chip8::Chip8() { this->pc = START_ADRESS; }

Chip8::~Chip8() {}

// Loads a ROM file into the ROM section of the memory
// 0x200 ROM Address
void Chip8::loadROM(char const *path) {
  std::ifstream ROM(path, std::ios::binary | std::ios::ate);

  if (ROM.is_open()) {

    std::streamoff size = ROM.tellg();
    char *buffer = new char[size];
    ROM.seekg(0, std::ios::beg);
    ROM.read(buffer, size);
    ROM.close();

    memcpy((void *)&this->memory[START_ADRESS], (void *)buffer, size);
    delete[] buffer;
  }
}
