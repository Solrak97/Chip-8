#include "chip.hpp"
#include <algorithm>
#include <cstring>
#include <fstream>

#include <iostream>
using namespace std;

const unsigned int START_ADRESS = 0x200;
const unsigned int FONTSET_SIZE = 80;
const unsigned int FONTSET_START_ADDRESS = 0x50;

uint8_t fontset[FONTSET_SIZE] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

Chip8::Chip8() {
  // Set emulator vars
  std::fill(this->registers, this->registers + 16, 0);
  std::fill(this->memory, this->memory + 4096, 0);
  this->index = 0;
  this->pc = 0;
  std::fill(this->stack, this->stack + 16, 0);
  this->sp = 0;
  this->delayTimer = 0;
  this->soundTimer = 0;
  std::fill(this->video, this->video + 2048, 0);
  this->opcode = 0;
  this->pc = START_ADRESS;

  // Set fonts on memory
  memcpy(&memory[FONTSET_START_ADDRESS], fontset, FONTSET_SIZE);

  // Set random seed
  randByte = std::uniform_int_distribution<uint8_t>(0, 255u);
}

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

void Chip8::Tick() {
  // Fetch instruction
  this->opcode = (this->memory[pc]);
  // inc PC
  this->pc += 2;
  // Decode
  // TO-DO
  // Decrement delay timer
  if (this->delayTimer > 0) {
    --this->delayTimer;
  }

  // Decrement sound timer
  if (this->soundTimer > 0) {
    --this->soundTimer;
  }
}

// Chip instructions
void Chip8::OP_00E0() { memset(this->video, 0, sizeof(this->video)); }

void Chip8::OP_00EE() {
  --this->sp;
  this->pc = this->stack[sp];
}

void Chip8::OP_1nnn() {
  uint16_t address = this->opcode & 0x0FFFu;
  this->pc = address;
}

void Chip8::OP_2nnn() {
  this->stack[sp++] = pc;
  OP_1nnn();
}

void Chip8::OP_3xkk() {
  uint8_t x = (this->opcode & 0x0F00) >> 8u;
  uint8_t kk = (this->opcode & 0x0FF);

  if (this->registers[x] == kk) {
    this->pc += 2;
  }
}

void Chip8::OP_4xkk() {
  uint8_t x = (this->opcode & 0x0F00) >> 8u;
  uint8_t kk = (this->opcode & 0x0FF);

  if (this->registers[x] != kk) {
    this->pc += 2;
  }
}

void Chip8::OP_5xy0() {
  uint8_t x = (this->opcode & 0x0F00) >> 8u;
  uint8_t y = (this->opcode & 0x00F0) >> 4u;

  if (this->registers[x] == this->registers[y]) {
    this->pc += 2;
  }
}
void Chip8::OP_6xkk() {
  uint8_t x = (this->opcode & 0x0F00) >> 8u;
  uint8_t kk = (this->opcode & 0x0FF);

  this->registers[x] = kk;
}

void Chip8::OP_7xkk() {
  uint8_t x = (this->opcode & 0x0F00) >> 8u;
  uint8_t kk = (this->opcode & 0x0FF);
  this->registers[x] += kk;
}

void Chip8::OP_8xy0() {
  uint8_t x = (this->opcode & 0x0F00) >> 8u;
  uint8_t y = (this->opcode & 0x00F0) >> 4u;

  this->registers[x] = this->registers[y];
}

void Chip8::OP_8xy1() {
  uint8_t x = (this->opcode & 0x0F00) >> 8u;
  uint8_t y = (this->opcode & 0x00F0) >> 4u;

  this->registers[x] |= this->registers[y];
}

void Chip8::OP_8xy2() {
  uint8_t x = (this->opcode & 0x0F00) >> 8u;
  uint8_t y = (this->opcode & 0x00F0) >> 4u;

  this->registers[x] &= this->registers[y];
}

void Chip8::OP_8xy3() {
  uint8_t x = (this->opcode & 0x0F00) >> 8u;
  uint8_t y = (this->opcode & 0x00F0) >> 4u;

  this->registers[x] ^= this->registers[y];
}

void Chip8::OP_8xy4() {
  uint8_t x = (this->opcode & 0x0F00) >> 8u;
  uint8_t y = (this->opcode & 0x00F0) >> 4u;

  uint16_t sum = this->registers[x] + this->registers[y];
  this->registers[0xF] = sum > 255u;
  this->registers[x] = sum & 0xFFu;
}

void Chip8::OP_8xy5() {
  uint8_t x = (this->opcode & 0x0F00) >> 8u;
  uint8_t y = (this->opcode & 0x00F0) >> 4u;

  this->registers[0xF] = this->registers[x] > this->registers[y];
  this->registers[x] -= this->registers[y];
}

void Chip8::OP_8xy6() {
  uint8_t x = (this->opcode & 0x0F00) >> 8u;

  this->registers[0xF] = this->registers[x] & 0xF1u;
  this->registers[x] >>= 1;
}

void Chip8::OP_8xy7() {
  uint8_t x = (this->opcode & 0x0F00) >> 8u;
  uint8_t y = (this->opcode & 0x00F0) >> 4u;

  this->registers[0xF] = this->registers[y] > this->registers[x];
  this->registers[x] = this->registers[y] - this->registers[x];
}

void Chip8::OP_8xyE() {
  uint8_t x = (this->opcode & 0x0F00) >> 8u;
  this->registers[0xF] = (this->registers[x] & 0x80u) >> 7u;
  this->registers[x] <<= 1;
}

void Chip8::OP_9xy0() {} // SNE Vx, Vy
void Chip8::OP_Annn() {} // LD I, addr
void Chip8::OP_Bnnn() {} // JP V0, addr
void Chip8::OP_Cxkk() {} // RND Vx, byte
void Chip8::OP_Dxyn() {} // DRW Vx, Vy, nibble
void Chip8::OP_Ex9E() {} // SKP Vx
void Chip8::OP_ExA1() {} // SKNP Vx
void Chip8::OP_FX07() {} // LD Vx, DT
void Chip8::OP_FX0A() {} // LD Vx, K
void Chip8::OP_Fx15() {} // LD DT, Vx
void Chip8::OP_Fx18() {} // LD ST, Vx
void Chip8::OP_Fx1E() {} // ADD I, Vx
void Chip8::OP_Fx29() {} // LD F, Vx
void Chip8::OP_Fx33() {} // LD B, Vx
void Chip8::OP_Fx55() {} // LD [I], Vx
void Chip8::OP_Fx65() {} // LD Vx, [I]
void Chip8::OP_NOOP() {} // NO OP
