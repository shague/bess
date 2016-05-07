#ifndef _COMMON_H_
#define _COMMON_H_

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#define ct_assert(p)	_Static_assert(p, "Compile-time assertion failure")

/* For minor performance optimization. Same as _nassert() of TI compilers */
#define promise(cond) do { if (!(cond)) __builtin_unreachable(); } while (0)

extern const struct global_opts {
	uint16_t port;		/* TCP port for controller (0 for default) */
	int default_core;	/* Core ID for implicily launched worker */
	int foreground;		/* If 1, not daemonized */
	int kill_existing;	/* If 1, kill existing BESS instance */
	int print_tc_stats;	/* If 1, print TC stats every second */
	int debug_mode;		/* If 1, print control messages */
	int mb_per_socket;	/* MB per CPU socket for DPDK (0=default) */
	char *pidfile;		/* Filename (nullptr=default; nullstr=none) */
	int multi_instance;	/* If 1, allow multiple BESS instances */
} global_opts;

#define member_type(type, member) typeof(((type *)0)->member)

#define container_of(ptr, type, member) \
	((type *)((char *)(member_type(type, member) *){ptr} - \
		offsetof(type, member)))

#define MIN(a, b) \
	({ \
		__typeof__ (a) _a = (a); \
		__typeof__ (b) _b = (b); \
	 	_a <= _b ? _a : _b; \
	 })

#define MAX(a, b) \
	({ \
		__typeof__ (a) _a = (a); \
		__typeof__ (b) _b = (b); \
	 	_a >= _b ? _a : _b; \
	 })

#define ACCESS_ONCE(x) (*(volatile typeof(x) *)&(x))

/* err is defined as -errno,  */
static inline intptr_t ptr_to_err(const void *ptr)
{
	return (intptr_t) ptr;
}

static inline void *err_to_ptr(intptr_t err)
{
	return (void *) err;
}

static inline int is_err(const void *ptr)
{
	const int max_errno = 4095;
	return (uintptr_t)ptr >= (uintptr_t)-max_errno;
}

static inline int is_err_or_null(const void *ptr)
{
	return !ptr || is_err(ptr);
}

#define __cacheline_aligned __attribute__((aligned(64)))

/* For x86_64. DMA operations are not safe with these macros */
#define INST_BARRIER()		asm volatile("" ::: "memory")
#define LOAD_BARRIER()		INST_BARRIER()
#define STORE_BARRIER()		INST_BARRIER()
#define FULL_BARRIER()		asm volatile("mfence":::"memory")

#endif
