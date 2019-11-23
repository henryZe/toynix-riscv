#include <kernel/param.h>
#include <types.h>

uint8_t stack0[KSTACKSIZE * NCPU] __attribute__ ((aligned (16)));

// entry.S jumps here in machine mode on stack0.
void start()
{

}
