#include <kernel/param.h>
#include <kernel/riscv.h>
#include <kernel/memlayout.h>
#include <types.h>

// entry.S needs one stack per CPU.
uint8 stack0[KSTACKSIZE * NCPU] __attribute__ ((aligned (16)));

// scratch area for timer interrupt, one per CPU.
uint64 mscratch0[NCPU * 32];

void main(void);
void timerinit(void);
void timervec(void);

// entry.S jumps here in machine mode on stack0.
void start(void)
{
	// set M Previous Privilege mode to Supervisor, for mret.
	unsigned long x = r_mstatus();
	x &= ~MSTATUS_MPP_MASK;
	x |= MSTATUS_MPP_S;
	w_mstatus(x);

	// set M Exception Program Counter to main, for mret.
	// requires gcc -mcmodel=medany
	w_mepc((uint64)main);

	// disable paging for now.
	w_satp(0);

	// delegate all interrupts and exceptions to supervisor mode.
	w_medeleg(0xffff);
	w_mideleg(0xffff);

	// ask for clock interrupts
	timerinit();

	// keep each CPU's hartid in its tp register, for cpuid().
	int id = r_mhartid();
	w_tp(id);

	// switch to supervisor mode and jump to main().
	asm volatile("mret");
}

// set up to receive timer interrupts in machine mode,
// which arrive at timervec in kernelvec.S,
// which turns them into software interrupts for
// devintr() in trap.c.
void timerinit(void)
{
	// each CPU has a separate source of timer interrupts.
	int id = r_mhartid();

	// ask the CLINT for a timer interrupt.
	int interval = 1000000; // cycles; about 1/10th second in qemu.
	*(uint64 *)CLINT_MTIMECMP(id) = *(uint64 *)CLINT_MTIME + interval;

	// prepare information in scratch[] for timervec.
	// scratch[0..3] : space for timervec to save registers.
	// scratch[4] : address of CLINT MTIMECMP register.
	// scratch[5] : desired interval (in cycles) between timer interrupts.
	uint64 *scratch = &mscratch0[32 * id];
	scratch[4] = CLINT_MTIMECMP(id);
	scratch[5] = interval;
	w_mscratch((uint64)scratch);

	// set the machine-mode trap handler.
	w_mtvec((uint64)timervec);

	// enable machine-mode interrupts.
	w_mstatus(r_mstatus() | MSTATUS_MIE);

	// enable machine-mode timer interrupts.
	w_mie(r_mie() | MIE_MTIE);
}
