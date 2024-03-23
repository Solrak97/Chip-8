#include <cstdint>

class Chip8 {
public:
  uint8_t registers[16] = {};
  uint8_t memory[4096] = {};
  uint16_t index = 0;
  uint16_t pc = 0;
  uint16_t stack[16] = {};
  uint8_t sp = 0;
  uint8_t delayTimer = 0;
  uint8_t soundTimer = 0;
  uint32_t video[64 * 32] = {};
  uint16_t opcode = 0;

  Chip8();
  ~Chip8();
  void loadROM(char const *path);
};
