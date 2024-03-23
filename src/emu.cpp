#include "chip.hpp"
#include <iostream>

int main(int argc, char **argv) {
  int offset = 0x1FB;
  Chip8 chip = Chip8();

  chip.loadROM("gattoselacome.bin");

  for (int i = 0; i < 100; i++) {
    std::cout << chip.memory[i + offset];
  }
}
