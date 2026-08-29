#include "Cpu.h"
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>
int main(void)
{
    cpu_struct* cpu = cpu_create();
    if (!cpu){
        printf("%s\n", "Calloc failed");
        return 1;
    }
    cpu->memory_buffer[0x0] = 0xC6;
    cpu->memory_buffer[0x1] = 0x01;
    cpu->memory_buffer[0x2] = 0x0C; 
    cpu->memory_buffer[0x3] = 0x00; 
    cpu->memory_buffer[0x4] = 0x11; 
    cpu->memory_buffer[0x5] = 0xF4;
    // ^^^ mov ah, 12 add ah, ah
    // = 24 for those who dont know math (like me)
    cpu_loop(cpu);
    cpu_clean(cpu);
}