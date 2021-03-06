#include "cpu.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define DATA_LEN 6

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */

// struct cpu
// {
//   unsigned char pc;
//   unsigned char registry[8];
//   unsigned char ram[256];
// };

void cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value){
  cpu->ram[address] = value;
}

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address){
  return cpu->ram[address];
}


void cpu_load(struct cpu *cpu, char filename[])
{
  FILE *fptr;
  char *ptr;
  char buffer[255];
  char cmdString[9];
  
  unsigned int address = 0;
  
  fptr = fopen(filename ? filename : "examples/mult.ls8", "r");
  if(fptr){
   while(fgets(buffer,255,fptr)){
    if(buffer){
      //strncpy(cmdString,buffer,8);
      unsigned char cmd = strtoul(buffer, &ptr ,2);
      cpu->ram[address++] = cmd;    
    }        
  }
  } else {
    printf("Null");
  }

  //Print Ram Values/Addresses
  // for(int i = 0; i < 45; i++){
  //   printf("%d  %d\n",i, cpu->ram[i]);
  // }
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op) {
    case ALU_MUL:
      // TODO
      break;

    // TODO: implement more ALU ops
  }
}

void push(struct cpu *cpu, unsigned char operandA){
  //printf("%s %d\n","PUSH",cpu->registry[operandA]);
  cpu->SP -= 1;
  cpu->ram[cpu->SP] = cpu->registry[operandA];   
}
unsigned char pop(struct cpu *cpu){
  return cpu_ram_read(cpu, cpu->SP++);
}

void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction
  while (running && cpu->pc < 150) {
    unsigned char instruction = cpu_ram_read(cpu, cpu->pc);
    unsigned int operand = instruction >> 6;
    unsigned char operandA =  cpu_ram_read(cpu, cpu->pc+1);
    unsigned char operandB =  cpu_ram_read(cpu, cpu->pc+2);
    
    switch (instruction)
    {
      case HLT:
        running = 0;
        break;
      case LDI:
        cpu->registry[operandA] = operandB;
        break;
      case PRN:
        printf("%s %d %s %d\n", "PRN:",cpu->registry[operandA], ", Register:", operandA);
        break;
      case MUL:
        cpu->registry[operandA] *= cpu->registry[operandB];
        break;
      case PUSH:
        push(cpu,operandA);
        break;
      case POP:        
        cpu->registry[operandA] = pop(cpu);
        break;
      case CALL:
        cpu->ram[--cpu->SP] = cpu->pc + 1 + operand;
        cpu->pc = cpu->registry[operandA];
        continue;
      case RET:
        cpu->pc = pop(cpu); 
        continue;           
      case ADD:
        cpu->registry[operandA] += cpu->registry[operandB];
      default:
        break;
    }
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    // 2. Figure out how many operands this next instruction requires
    // 3. Get the appropriate value(s) of the operands following this instruction
    // 4. switch() over it to decide on a course of action.
    // 5. Do whatever the instruction should do according to the spec.
    // 6. Move the PC to the next instruction.
  
  cpu->pc += 1 + operand;
  
  }
  
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  memset(cpu->ram, 0, sizeof cpu->ram);
  memset(cpu->registry,0,sizeof cpu->registry);
  cpu->pc = 0;
  cpu->SP = 0xf4;
}
