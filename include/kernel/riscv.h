#include <types.h>

// which hart (core) is this?
static inline uint64 r_mhartid()
{
	uint64 x;
	asm volatile("csrr %0, mhartid" : "=r" (x));
	return x;
}

#define MSTATUS_MPP_MASK (3L << 11) // previous mode.
#define MSTATUS_MPP_M (3L << 11)
#define MSTATUS_MPP_S (1L << 11)
#define MSTATUS_MPP_U (0L << 11)
#define MSTATUS_MIE (1L << 3)       // machine-mode interrupt enable.

static inline uint64 r_mstatus()
{
    uint64 x;
    asm volatile("csrr %0, mstatus" : "=r" (x));
    return x;
}

static inline void w_mstatus(uint64 x)
{
	asm volatile("csrw mstatus, %0" : : "r" (x));
}

// Machine-mode Interrupt Enable
#define MIE_MEIE (1L << 11) // external
#define MIE_MTIE (1L << 7)	// timer
#define MIE_MSIE (1L << 3)	// software

static inline uint64 r_mie()
{
	uint64 x;
	asm volatile("csrr %0, mie" : "=r" (x));
	return x;
}

static inline void w_mie(uint64 x)
{
	asm volatile("csrw mie, %0" : : "r" (x));
}

// machine exception program counter, holds the
// instruction address to which a return from
// exception will go.
static inline void w_mepc(uint64 x)
{
	asm volatile("csrw mepc, %0" : : "r" (x));
}

// Machine Exception Delegation
static inline uint64 r_medeleg()
{
	uint64 x;
	asm volatile("csrr %0, medeleg" : "=r" (x));
	return x;
}

static inline void w_medeleg(uint64 x)
{
	asm volatile("csrw medeleg, %0" : : "r" (x));
}

// Machine Interrupt Delegation
static inline uint64 r_mideleg()
{
	uint64 x;
	asm volatile("csrr %0, mideleg" : "=r" (x));
	return x;
}

static inline void w_mideleg(uint64 x)
{
	asm volatile("csrw mideleg, %0" : : "r" (x));
}

static inline void w_mscratch(uint64 x)
{
	asm volatile("csrw mscratch, %0" : : "r" (x));
}

// Machine-mode interrupt vector
static inline void w_mtvec(uint64 x)
{
	asm volatile("csrw mtvec, %0" : : "r" (x));
}

// supervisor address translation and protection;
// holds the address of the page table.
static inline uint64 r_satp()
{
	uint64 x;
	asm volatile("csrr %0, satp" : "=r" (x));
	return x;
}

static inline void w_satp(uint64 x)
{
	asm volatile("csrw satp, %0" : : "r" (x));
}

// read and write tp, the thread pointer, which holds
// this core's hartid (core number), the index into cpus[].
static inline uint64 r_tp()
{
	uint64 x;
	asm volatile("mv %0, tp" : "=r" (x));
	return x;
}

static inline void w_tp(uint64 x)
{
	asm volatile("mv tp, %0" : : "r" (x));
}
