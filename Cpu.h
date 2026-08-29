#pragma once
#include <stdint.h>
#define MEMORY_SIZE 2ULL * 1024 * 1024 * 1024
typedef struct {
    union {
        uint32_t eax;
        struct {uint8_t al, ah;};
    };
    union {
        uint32_t ecx;
        struct {uint8_t cl, ch;};
    };
    union {
        uint32_t edx;
        struct {uint8_t dl, dh;};
    };
    union {
        uint32_t ebx;
        struct {uint8_t bl, bh;};
    };
    uint32_t esp, ebp, esi, edi;
    uint16_t cs, ds, es, fs, gs, ss;
} registers_t;
typedef struct {
    int CF, PF, AF, ZF, SF, OF;
} flags_t;
typedef struct {
    flags_t flags;
    registers_t registers;
    uint32_t program_counter;
    uint8_t* memory_buffer;
    uint8_t running_boolean;
} cpu_struct;
cpu_struct* cpu_create();
void cpu_loop(cpu_struct* cpu);
void cpu_clean(cpu_struct* cpu);
