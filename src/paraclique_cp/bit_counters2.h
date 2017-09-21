// John Eblen
// August 2, 2006
// An array that maps byte i to the number of 1 bits in that byte.
// NOTE:  This header contains some architecture-specific definitions
// (see below)

#ifndef _BIT_COUNTERS_H
#define _BIT_COUNTERS_H 1

// Architecture dependent - uint2b must be a 2-byte unsigned integer
typedef unsigned short int uint2b;

int count_ones(unsigned long ul);
int count_ones(unsigned long *, int);
int count_ones_fast(unsigned long *, int);

#endif
