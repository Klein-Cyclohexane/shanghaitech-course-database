#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "cache.h"
#include "simulate.h"

#define REGISTER_NAME_TO_NUMBER(name1, name2, number) \
  if (strcmp(register_name, name1) == 0 || strcmp(register_name, name2) == 0) return number

#define PARSE_LW(inst_name) \
  else if (strcmp(inst, #inst_name) == 0) { \
    instruction->type = INSTRUCTION_ ## inst_name; \
    instruction->rd = register_name_to_label(strtok(NULL, ", ")); \
    instruction->imm = strtol(strtok(NULL, " ("), NULL, 10); \
    instruction->rs1 = register_name_to_label(strtok(NULL, ")")); \
  }

#define PARSE_SW(inst_name) \
  else if (strcmp(inst, #inst_name) == 0) { \
    instruction->type = INSTRUCTION_ ## inst_name; \
    instruction->rs2 = register_name_to_label(strtok(NULL, ", ")); \
    instruction->imm = strtol(strtok(NULL, " ("), NULL, 10); \
    instruction->rs1 = register_name_to_label(strtok(NULL, ")")); \
  }

#define PARSE_RTYPE(inst_name) \
  else if (strcmp(inst, #inst_name) == 0) { \
    instruction->type = INSTRUCTION_ ## inst_name; \
    instruction->rd = register_name_to_label(strtok(NULL, ", ")); \
    instruction->rs1 = register_name_to_label(strtok(NULL, ", ")); \
    instruction->rs2 = register_name_to_label(strtok(NULL, " \n")); \
  }

#define PARSE_ITYPE(inst_name) \
  else if (strcmp(inst, #inst_name) == 0) { \
    instruction->type = INSTRUCTION_ ## inst_name; \
    instruction->rd = register_name_to_label(strtok(NULL, ", ")); \
    instruction->rs1 = register_name_to_label(strtok(NULL, ", ")); \
    instruction->imm = strtol(strtok(NULL, " \n"), NULL, 0); \
  }

#define PARSE_SBTYPE(inst_name) \
  else if (strcmp(inst, #inst_name) == 0) { \
    instruction->type = INSTRUCTION_ ## inst_name; \
    instruction->rs1 = register_name_to_label(strtok(NULL, ", ")); \
    instruction->rs2 = register_name_to_label(strtok(NULL, ", ")); \
    instruction->imm = strtol(strtok(NULL, " \n"), NULL, 0); \
  }

/* 32 integer registers. */
static uint32_t registers[32];

static uint32_t program_counter;

static unsigned register_name_to_label(const char *register_name) {
  REGISTER_NAME_TO_NUMBER("zero", "x0", 0);
  REGISTER_NAME_TO_NUMBER("ra", "x1", 1);
  REGISTER_NAME_TO_NUMBER("sp", "x2", 2);
  REGISTER_NAME_TO_NUMBER("gp", "x3", 3);
  REGISTER_NAME_TO_NUMBER("tp", "x4", 4);
  REGISTER_NAME_TO_NUMBER("t0", "x5", 5);
  REGISTER_NAME_TO_NUMBER("t1", "x6", 6);
  REGISTER_NAME_TO_NUMBER("t2", "x7", 7);
  REGISTER_NAME_TO_NUMBER("s0", "x8", 8);
  REGISTER_NAME_TO_NUMBER("fp", "x8", 8); 
  REGISTER_NAME_TO_NUMBER("s1", "x9", 9);
  REGISTER_NAME_TO_NUMBER("a0", "x10", 10);
  REGISTER_NAME_TO_NUMBER("a1", "x11", 11);
  REGISTER_NAME_TO_NUMBER("a2", "x12", 12);
  REGISTER_NAME_TO_NUMBER("a3", "x13", 13);
  REGISTER_NAME_TO_NUMBER("a4", "x14", 14);
  REGISTER_NAME_TO_NUMBER("a5", "x15", 15);
  REGISTER_NAME_TO_NUMBER("a6", "x16", 16);
  REGISTER_NAME_TO_NUMBER("a7", "x17", 17);
  REGISTER_NAME_TO_NUMBER("s2", "x18", 18);
  REGISTER_NAME_TO_NUMBER("s3", "x19", 19);
  REGISTER_NAME_TO_NUMBER("s4", "x20", 20);
  REGISTER_NAME_TO_NUMBER("s5", "x21", 21);
  REGISTER_NAME_TO_NUMBER("s6", "x22", 22);
  REGISTER_NAME_TO_NUMBER("s7", "x23", 23);
  REGISTER_NAME_TO_NUMBER("s8", "x24", 24);
  REGISTER_NAME_TO_NUMBER("s9", "x25", 25);
  REGISTER_NAME_TO_NUMBER("s10", "x26", 26);
  REGISTER_NAME_TO_NUMBER("s11", "x27", 27);
  REGISTER_NAME_TO_NUMBER("t3", "x28", 28);
  REGISTER_NAME_TO_NUMBER("t4", "x29", 29);
  REGISTER_NAME_TO_NUMBER("t5", "x30", 30);
  REGISTER_NAME_TO_NUMBER("t6", "x31", 31);
  
  fprintf(stderr, "Warning: Unknown or unmapped register name '%s'\n", register_name);
  return 32; /* Return 32 on error. */
}

static void parse_inst(char *buf, struct Instruction *instruction) {
  char *inst = strtok(buf, " ");
  if (strcmp(inst, "li") == 0) {
    instruction->type = INSTRUCTION_li;
    instruction->rd = register_name_to_label(strtok(NULL, ", "));
    instruction->imm = strtol(strtok(NULL, " \n"), NULL, 0);
  }
  PARSE_LW(lw)
  PARSE_SW(sw)
  PARSE_RTYPE(add)
  PARSE_RTYPE(sub)
  PARSE_RTYPE(sll)
  PARSE_RTYPE(xor)
  PARSE_RTYPE(srl)
  PARSE_RTYPE(or)
  PARSE_RTYPE(and)
  PARSE_RTYPE(mul)
  PARSE_RTYPE(div)
  PARSE_RTYPE(rem)
  PARSE_ITYPE(addi)
  PARSE_ITYPE(xori)
  PARSE_ITYPE(ori)
  PARSE_ITYPE(andi)
  PARSE_ITYPE(slli)
  PARSE_ITYPE(srli)
  PARSE_SBTYPE(beq)
  PARSE_SBTYPE(bne)

}

// Reads assembly from a file and parses it into an array of Instruction structs
struct Instruction *parse_asm(FILE *file, size_t num_lines) {
  if (num_lines == 0) {
      return NULL;
  }
  struct Instruction *instructions = malloc(sizeof(struct Instruction) * num_lines);
  if (!instructions) {
    perror("Error: Failed to allocate memory for instructions in parse_asm");
    exit(EXIT_FAILURE); // Critical error
  }

  char buf[100]; // Buffer for each line
  for (size_t i = 0; i < num_lines; ++i) {
    if (fgets(buf, sizeof(buf), file) == NULL) {
      fprintf(stderr, "Error: fgets failed to read line %zu. Expected %zu lines.\n", i + 1, num_lines);
      free(instructions); // Clean up allocated memory
      exit(EXIT_FAILURE); // Critical error, input does not match expectations
    }
    parse_inst(buf, &instructions[i]);
  }
  return instructions;
}


void start_simulation(struct Instruction *instructions, size_t num_static_instructions) {
  memset(registers, 0, sizeof(registers));
  program_counter = 0; // program_counter is the index for the 'instructions' array
  
  registers[0] = 0; // x0 is hardwired to zero

  while (program_counter < num_static_instructions) {
    add_inst_access(program_counter * 4); // Cache access for instruction fetch (byte address)
    
    struct Instruction *inst = &instructions[program_counter];
    uint32_t current_pc_idx = program_counter; // PC index of the current instruction
    uint32_t next_pc_idx = current_pc_idx + 1;   // Default: advance to next instruction

    uint32_t addr; // For lw/sw addresses

    switch (inst->type) {
      case INSTRUCTION_li:
        if (inst->rd != 0) registers[inst->rd] = inst->imm;
        break;
        
      case INSTRUCTION_lw:
        addr = registers[inst->rs1] + inst->imm;
        add_data_access(addr);
        break;
        
      case INSTRUCTION_sw:
        addr = registers[inst->rs1] + inst->imm;
        add_data_access(addr);
        break;
        
      // R-Type Instructions
      case INSTRUCTION_add:
        if (inst->rd != 0) registers[inst->rd] = registers[inst->rs1] + registers[inst->rs2];
        break;
      case INSTRUCTION_sub:
        if (inst->rd != 0) registers[inst->rd] = registers[inst->rs1] - registers[inst->rs2];
        break;
      case INSTRUCTION_sll: // op rd, rs1, rs2
        if (inst->rd != 0) registers[inst->rd] = registers[inst->rs1] << (registers[inst->rs2] & 0x1F); // Use lower 5 bits of rs2
        break;
      case INSTRUCTION_xor:
        if (inst->rd != 0) registers[inst->rd] = registers[inst->rs1] ^ registers[inst->rs2];
        break;
      case INSTRUCTION_srl: // op rd, rs1, rs2
        if (inst->rd != 0) registers[inst->rd] = registers[inst->rs1] >> (registers[inst->rs2] & 0x1F); // Use lower 5 bits of rs2
        break;
      case INSTRUCTION_or:
        if (inst->rd != 0) registers[inst->rd] = registers[inst->rs1] | registers[inst->rs2];
        break;
      case INSTRUCTION_and:
        if (inst->rd != 0) registers[inst->rd] = registers[inst->rs1] & registers[inst->rs2];
        break;
      case INSTRUCTION_mul:
        if (inst->rd != 0) registers[inst->rd] = registers[inst->rs1] * registers[inst->rs2];
        break;
      case INSTRUCTION_div:
        if (inst->rd != 0) {
          if (registers[inst->rs2] == 0) {
            registers[inst->rd] = 0xFFFFFFFF; // RISC-V: divisor is zero, result is all ones
          } else if (registers[inst->rs1] == 0x80000000 && registers[inst->rs2] == 0xFFFFFFFF) {
             // RISC-V: overflow (INT_MIN / -1), result is INT_MIN (0x80000000)
            registers[inst->rd] = 0x80000000;
          }
          else {
            registers[inst->rd] = (int32_t)registers[inst->rs1] / (int32_t)registers[inst->rs2];
          }
        }
        break;
      case INSTRUCTION_rem:
        if (inst->rd != 0) {
          if (registers[inst->rs2] == 0) {
            registers[inst->rd] = registers[inst->rs1]; // RISC-V: divisor is zero, result is dividend
          } else if (registers[inst->rs1] == 0x80000000 && registers[inst->rs2] == 0xFFFFFFFF) {
            // RISC-V: overflow (INT_MIN % -1), result is 0
            registers[inst->rd] = 0;
          }
          else {
            registers[inst->rd] = (int32_t)registers[inst->rs1] % (int32_t)registers[inst->rs2];
          }
        }
        break;

      // I-Type Instructions
      case INSTRUCTION_addi:
        if (inst->rd != 0) registers[inst->rd] = registers[inst->rs1] + inst->imm;
        break;
      case INSTRUCTION_xori:
        if (inst->rd != 0) registers[inst->rd] = registers[inst->rs1] ^ inst->imm; // Immediate is sign-extended for logic ops in HW, but C handles int promotion.
        break;
      case INSTRUCTION_ori:
        if (inst->rd != 0) registers[inst->rd] = registers[inst->rs1] | inst->imm;
        break;
      case INSTRUCTION_andi:
        if (inst->rd != 0) registers[inst->rd] = registers[inst->rs1] & inst->imm;
        break;
      case INSTRUCTION_slli: // op rd, rs1, shamt
        if (inst->rd != 0) registers[inst->rd] = registers[inst->rs1] << (inst->imm & 0x1F); // shamt is lower 5 bits of imm for RV32
        break;
      case INSTRUCTION_srli: // op rd, rs1, shamt
        if (inst->rd != 0) registers[inst->rd] = registers[inst->rs1] >> (inst->imm & 0x1F); // shamt is lower 5 bits of imm for RV32
        break;

      case INSTRUCTION_beq:
        if (registers[inst->rs1] == registers[inst->rs2]) {

          next_pc_idx = current_pc_idx + (inst->imm / 4);
        }
        break;
      case INSTRUCTION_bne:
        if (registers[inst->rs1] != registers[inst->rs2]) {

          next_pc_idx = current_pc_idx + (inst->imm / 4);
        }
        break;
        
      default:
        break;
    }
    program_counter = next_pc_idx; 
    registers[0] = 0; 
  }
}