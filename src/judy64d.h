#ifndef __JUDY64d__
#define __JUDY64d__

typedef unsigned int uint;

#if defined(__LP64__) || \
	defined(__x86_64__) || \
	defined(__amd64__) || \
	defined(_WIN64) || \
	defined(__sparc64__) || \
	defined(__arch64__) || \
	defined(__powerpc64__) || \
	defined (__s390x__) 
	//	defines for 64 bit
	
	typedef unsigned long long judyvalue;
	typedef unsigned long long judyslot;
	#define JUDY_key_mask (0x07)
	#define JUDY_key_size 8
	#define JUDY_slot_size 8
	#define JUDY_span_bytes (3 * JUDY_key_size)

	#define PRIjudyvalue	"llu"

#else
	//	defines for 32 bit
	
	typedef uint judyvalue;
	typedef uint judyslot;
	#define JUDY_key_mask (0x03)
	#define JUDY_key_size 4
	#define JUDY_slot_size 4
	#define JUDY_span_bytes (7 * JUDY_key_size)

	#define PRIjudyvalue	"u"

#endif

typedef struct {
	void *seg;			// next used allocator
	uint next;			// next available offset
} JudySeg;

typedef struct {
	judyslot next;		// judy object
	uint off;			// offset within key
	int slot;			// slot within object
} JudyStack;

typedef struct {
	judyslot root[1];	// root of judy array
	void **reuse[8];	// reuse judy blocks
	JudySeg *seg;		// current judy allocator
	uint level;			// current height of stack
	uint max;			// max height of stack
	JudyStack stack[1];	// current cursor
} Judy;

#endif