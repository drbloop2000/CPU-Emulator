#include "Cpu.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
cpu_struct* cpu_create()
{
    cpu_struct* cpu = calloc(1, sizeof(cpu_struct));
    uint8_t* memory_buffer = malloc(MEMORY_SIZE);
    if (!cpu || !memory_buffer){
        return NULL;
    }
    cpu->memory_buffer = memory_buffer;
    return cpu;
}
void cpu_clean(cpu_struct* cpu)
{
    cpu->running_boolean = 0;
    free(cpu->memory_buffer);
    free(cpu);
    // cpu = NULL;
    return;
}
static inline void update_alu(cpu_struct* cpu, uint8_t a, uint8_t b, const uint8_t result, const uint8_t opcode)
{
    // TODO: when more opcodes are added, this becomes a mess really quickly
    // we have a few options:
    // 1. cry about it and keep doing this anyway
    // 2. make seperate inlined functions ex. updape_alu_add update_alu_XOR
    // 3. find a better solution..
    // ^^ somehwat complete this will do for now, maybe it could be improved?
    cpu->flags.ZF = (result == 0);
    cpu->flags.SF = (result >> 7) & 1;
    cpu->flags.AF = ((a ^ b ^ result) & 0x10) != 0;
    int res = 0;
    for (uint8_t i = result; i; i &= i - 1){
         res += 1;
    }
    cpu->flags.PF = (res & 1) == 0;
    switch (opcode)
    {
        case 0x0:   // add reg, reg
            cpu->flags.CF = (result < a);
            cpu->flags.OF = (~(a ^ b) & (a ^ result) & 0x80) != 0;
            break;
        case 0x28:  // sub reg, reg
            cpu->flags.CF = (a < b);
            cpu->flags.OF = ((a ^ b) & (a ^ result) & 0x80) != 0;
            break;
    }
    return;
}
void cpu_execute(cpu_struct* cpu, const uint8_t opcode)
{
    // printf("uhm opcode? %u\n", opcode);
    switch (opcode)
    {
        case 0x0:
        {
            // add reg, reg
            const uint8_t byte = cpu->memory_buffer[cpu->program_counter];
            const uint8_t low_bits = byte & 0x0F;
            const uint8_t high_bits = byte >> 4;
            uint8_t* register_ptr = (uint8_t*)&cpu->registers;
            const uint8_t a = register_ptr[low_bits];
            const uint8_t b = register_ptr[high_bits];
            const uint8_t result = (a + b);
            register_ptr[low_bits] = result;
            update_alu(cpu, a, b, result, opcode);
            cpu->program_counter += 1;
            break;
        }
        case 0x88:
            {
                // mov reg, reg
                const uint8_t byte = cpu->memory_buffer[cpu->program_counter];
                const uint8_t low_bits = byte & 0x0F;
                const uint8_t high_bits = byte >> 4;
                uint8_t* register_ptr = (uint8_t*)&cpu->registers;
                register_ptr[low_bits] = register_ptr[high_bits];
                cpu->program_counter += 1;
                break;
            }
        case 0xC6:
            {
                // mov reg, i
                const uint8_t byte = cpu->memory_buffer[cpu->program_counter];
                const uint8_t low_bits = byte & 0x0F;
                const uint8_t operand = cpu->memory_buffer[cpu->program_counter + 1];
                uint8_t* register_ptr = (uint8_t*)&cpu->registers;
                register_ptr[low_bits] = operand;
                cpu->program_counter += 2;
                break;
            }
        case 0x28:
            {
                // sub reg, reg
                const uint8_t byte = cpu->memory_buffer[cpu->program_counter];
                const uint8_t low_bits = byte & 0x0F;
                const uint8_t high_bits = byte >> 4;
                uint8_t* register_ptr = (uint8_t*)&cpu->registers;
                const uint8_t a = register_ptr[low_bits];
                const uint8_t b = register_ptr[high_bits];
                const uint8_t result = (a - b);
                register_ptr[low_bits] = result;
                update_alu(cpu, a, b, result, opcode);
                cpu->program_counter += 1;
                break;
            }
        case 0xF4:
            // halt
            cpu->running_boolean = 0;
            break;
        case 0x90:
        {
            // nop
            // literally do nothing lol
            break;
        }
        default:
            printf("Illegal OP: %u\n", opcode);
            cpu->running_boolean = 0;
            abort();
            break;
    }
    return;
}
void cpu_loop(cpu_struct* cpu)
{
    cpu->running_boolean = 1;
    while (cpu != NULL && cpu->running_boolean)
    {
       if (cpu->program_counter >= MEMORY_SIZE){
          cpu->running_boolean = 0;
          printf("%s\n", "CPU Out of Memory!");
          abort();
          return;
       }
       const uint8_t opcode = cpu->memory_buffer[cpu->program_counter]; 
       cpu->program_counter += 1;
       cpu_execute(cpu, opcode);
    }
    return;
}