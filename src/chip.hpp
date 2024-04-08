#include <chrono>
#include <cstdint>
#include <random>

class Chip8 {
public:
  // Emulation vars
  uint8_t registers[16];
  uint8_t memory[4096];
  uint16_t index;
  uint16_t pc;
  uint16_t stack[16];
  uint8_t sp;
  uint8_t delayTimer;
  uint8_t soundTimer;
  uint32_t video[64 * 32];
  uint16_t opcode;

  // Aux vars
  std::default_random_engine randGen;
  std::uniform_int_distribution<uint8_t> randByte;

  // Chip instructions
  void OP_00E0(); // CLS
  void OP_00EE(); // RET
  void OP_1nnn(); // JP addr
  void OP_2nnn(); // CALL addr
  void OP_3xkk(); // SE Vx, byte
  void OP_4xkk(); // SNE Vx, byte
  void OP_5xy0(); // SE Vx, Vy
  void OP_6xkk(); // LD Vx, byte
  void OP_7xkk(); // ADD Vx, byte
  void OP_8xy0(); // LD Vx, Vy
  void OP_8xy1(); // OR Vx, Vy
  void OP_8xy2(); // AND Vx, Vy
  void OP_8xy3(); // XOR Vx, Vy
  void OP_8xy4(); // ADD Vx, Vy
  void OP_8xy5(); // SUB Vx, Vy
  void OP_8xy6(); // SHR Vx
  void OP_8xy7(); // SUBN Vx, Vy
  void OP_8xyE(); // SHL Vx, {, Vy}
  void OP_9xy0(); // SNE Vx, Vy
  void OP_Annn(); // LD I, addr
  void OP_Bnnn(); // JP V0, addr
  void OP_Cxkk(); // RND Vx, byte
  void OP_Dxyn(); // DRW Vx, Vy, nibble
  void OP_Ex9E(); // SKP Vx
  void OP_ExA1(); // SKNP Vx
  void OP_FX07(); // LD Vx, DT
  void OP_FX0A(); // LD Vx, K
  void OP_Fx15(); // LD DT, Vx
  void OP_Fx18(); // LD ST, Vx
  void OP_Fx1E(); // ADD I, Vx
  void OP_Fx29(); // LD F, Vx
  void OP_Fx33(); // LD B, Vx
  void OP_Fx55(); // LD [I], Vx
  void OP_Fx65(); // LD Vx, [I]
  void OP_NOOP(); // NO OP

  // code tick
  void Tick();

  // Constructor / Destructor

  Chip8();
  ~Chip8();
  void loadROM(char const *path);
};
