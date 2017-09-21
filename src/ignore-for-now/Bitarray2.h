// John Eblen
// September 20, 2005
// Bitarray library
// A more flexible bitset library to replace the C++ STL library

#ifndef _BITARRAY_H
#define _BITARRAY_H 1

#include <iostream>
#include <cstdlib>
#include "bit_counters2.h"
#include <cstring>
using namespace std;

#define NUM_ULBITS (sizeof(unsigned long) * 8)

// Macros for common translations that are good targets for later optimization
#define BIT2UL(b) ((b)/NUM_ULBITS)
#define BIT2ULPOS(b) ((b) % NUM_ULBITS)

class bitarray
{
  mutable int num_ones;  // Added July 23, 2007 to implement memoization.
                         // A negative value means that the current count is
                         // unknown.
  bool init;
  int num_bits;
  int num_uls;
  unsigned long leftover_reset;  // Variable used to reset unused bits on the
                                 // last UL if some operation, such as "not",
                                 // sets them.
  unsigned long *bits;

  public:

  // Constructors
  bitarray()
  {
    num_ones = -1;
    init = 0;
    num_bits = 0;
    num_uls = 0;
    leftover_reset = 0;
    bits = 0;
  }

  bitarray(int n)
  {
    if (n < 1)
    {
      cerr << "Error - bitarray initialized with < 1 bit." << endl;
      exit(EXIT_FAILURE);
    }
    num_ones = -1;
    init = 1;
    num_bits = n;
    num_uls = n / NUM_ULBITS;
    if (n % NUM_ULBITS != 0) num_uls++;
    bits = new unsigned long[num_uls];
    reset();

    leftover_reset = 0;
    leftover_reset = ~leftover_reset;
    if (BIT2ULPOS(num_bits) != 0) // Bug fix - 2/19/07
        leftover_reset <<= (NUM_ULBITS - BIT2ULPOS(num_bits));
  }

  bitarray(const bitarray &ba)
  {
    num_ones = ba.num_ones;
    init = ba.init;
    num_bits = ba.num_bits;
    num_uls = ba.num_uls;
    leftover_reset = ba.leftover_reset;
    if (init)
    {
      bits = new unsigned long[num_uls];
      memcpy(bits, ba.bits, num_uls * NUM_ULBITS / 8);
    }
    else {bits = 0;}
  }

  // Assignment operator
  bitarray &operator=(const bitarray &ba)
  {
    if (init) delete [] bits;
    if (ba.init)
    {
      bits = new unsigned long[ba.num_uls];
      memcpy(bits, ba.bits, ba.num_uls * NUM_ULBITS / 8);
    }
    else {bits = 0;}

    num_ones = ba.num_ones;
    init = ba.init;
    num_bits = ba.num_bits;
    num_uls = ba.num_uls;
    leftover_reset = ba.leftover_reset;

    return *this;
  }

  // Destructor
  ~bitarray() {if (init) delete [] bits;}

  // Accessor functions
  bool ready() const {return init;}
  int size() const {return num_bits;}
  int count() const;

  bool operator[] (int) const;

  // Initializations
  void reset();
  void set();

  // Copy of integer constructor for uninitialized bitarrays
  void create(int n)
  {
    if (init) return;
    if (n < 1)
    {
      cerr << "Error - bitarray initialized with < 1 bit." << endl;
      exit(EXIT_FAILURE);
    }
    num_ones = -1;
    init = 1;
    num_bits = n;
    num_uls = n / NUM_ULBITS;
    if (n % NUM_ULBITS != 0) num_uls++;
    bits = new unsigned long[num_uls];
    reset();

    leftover_reset = 0;
    leftover_reset = ~leftover_reset;
    if (BIT2ULPOS(num_bits) != 0) // Bug fix - 2/19/07
        leftover_reset <<= (NUM_ULBITS - BIT2ULPOS(num_bits));
  }

  // Single Bit manipulations
  void flip(int);
  void reset(int);
  void set(int);

  // Logical operations
  bitarray &operator&=(const bitarray &ba);
  bitarray &operator|=(const bitarray &ba);
  bitarray &operator^=(const bitarray &ba);
  bitarray &flip();
  bitarray operator&(const bitarray &ba) const
  {
    bitarray tba(*this);
    tba &= ba;
    return tba;
  }

  bitarray operator|(const bitarray &ba) const
  {
    bitarray tba(*this);
    tba |= ba;
    return tba;
  }

  bitarray operator^(const bitarray &ba) const
  {
    bitarray tba(*this);
    tba ^= ba;
    return tba;
  }

  bitarray operator~() const
  {
    bitarray tba(*this);
    tba.flip();
    return tba;
  }

  // Comparison operator
  bool operator==(const bitarray &ba) const;
};

#endif
