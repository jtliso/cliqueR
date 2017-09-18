#include "Bitarray2.h"

bool bitarray::operator[] (int b) const
{
  unsigned long result = 1;

  result <<= (NUM_ULBITS - BIT2ULPOS(b) - 1);
  result &= bits[BIT2UL(b)];

  if (result) return 1;
  return 0;
}

void bitarray::flip(int b)
{
  unsigned long flipper = 1;

  flipper <<= (NUM_ULBITS - BIT2ULPOS(b) - 1);
  bits[BIT2UL(b)] ^= flipper;
  num_ones = -1;
}

void bitarray::reset(int b)
{
  unsigned long flipper = 1;

  flipper <<= (NUM_ULBITS - BIT2ULPOS(b) - 1);
  bits[BIT2UL(b)] &= (~flipper);
  num_ones = -1;
}

void bitarray::set(int b)
{
  unsigned long flipper = 1;

  flipper <<= (NUM_ULBITS - BIT2ULPOS(b) - 1);
  bits[BIT2UL(b)] |= flipper;
  num_ones = -1;
}

int bitarray::count() const
{
  if (!init) return 0;

  if (num_ones < 0) num_ones = count_ones_fast(bits, num_uls);
  return num_ones;
}

void bitarray::reset()
{
  int i;
  int num = num_uls;
  unsigned long *bits_ptr = bits;
  if (!init) return;
  if (num_ones == 0) return;

  for (i=0; i<num; i++) bits_ptr[i] = 0;
  num_ones = 0;
}

void bitarray::set()
{
  int i;
  int num = num_uls;
  unsigned long *bits_ptr = bits;
  if (!init) return;
  if (num_ones == num_bits) return;

  for (i=0; i<num; i++) {bits_ptr[i] = 0; bits_ptr[i] = ~bits_ptr[i];}
  bits[num_uls - 1] &= leftover_reset;
  num_ones = num_bits;
}

bitarray &bitarray::operator&=(const bitarray &ba)
{
  int i;
  int num = num_uls;
  unsigned long *bits_ptr = bits;
  unsigned long *ba_ptr = ba.bits;
  if (!init) return *this;

  for (i=0; i<num; i++) bits_ptr[i] &= ba_ptr[i];

  num_ones = -1;
  return *this;
}

bitarray &bitarray::operator|=(const bitarray &ba)
{
  int i;
  int num = num_uls;
  unsigned long *bits_ptr = bits;
  unsigned long *ba_ptr = ba.bits;
  if (!init) return *this;

  for (i=0; i<num; i++) bits_ptr[i] |= ba_ptr[i];

  num_ones = -1;
  return *this;
}

bitarray &bitarray::operator^=(const bitarray &ba)
{
  int i;
  int num = num_uls;
  unsigned long *bits_ptr = bits;
  unsigned long *ba_ptr = ba.bits;
  if (!init) return *this;

  for (i=0; i<num; i++) bits_ptr[i] ^= ba_ptr[i];

  num_ones = -1;
  return *this;
}

bitarray &bitarray::flip()
{
  int i;
  int num = num_uls;
  unsigned long *bits_ptr = bits;
  if (!init) return *this;

  for (i=0; i<num; i++) bits_ptr[i] = ~bits_ptr[i];
  bits[num_uls - 1] &= leftover_reset;

  num_ones = -1;
  return *this;
}

bool bitarray::operator==(const bitarray &ba) const
{
  if ((!init) || (!ba.init)) return 0;
  if (num_bits != ba.num_bits) return 0;
  if (memcmp(bits, ba.bits, num_uls * NUM_ULBITS / 8) == 0) return 1;
  return 0;
}
