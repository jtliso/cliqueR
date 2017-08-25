/* utility.c */
/* Implement functions defined in utility.h */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>  /* memset */
#include <sys/time.h>  /* gettimeofday */
#include "utility.h"

/**** Timer ****/

/* Returns current time in seconds as a double value
   and the precision can be as accurate as microseconds
   (10^-6 second) 
 */
double get_cur_time() {
  struct timeval   tv;
  struct timezone  tz;
  double cur_time;
  gettimeofday(&tv, &tz);
  cur_time = tv.tv_sec + tv.tv_usec / 1000000.0;
  return cur_time;
}


/**** Set ****/
 
/* Malloc and initialize a Set, returns a pointer to it */
Set *set_make(unsigned int set_size)
{
  Set *S;
  int num_ints;

  S = (Set *) malloc(sizeof(Set));
  if (S == NULL) { perror("malloc"); exit(-1); }
  S->_set_size = set_size;
  S->_num_elements = 0;
  num_ints = set_size / (sizeof(unsigned int) * CHAR_BITS);
  if ((set_size % (sizeof(unsigned int) * CHAR_BITS)) != 0) num_ints++;
  S->_set = (unsigned int *) malloc(num_ints * sizeof(unsigned int));
  if (S->_set == NULL) { perror("malloc"); exit(-1); }
  memset(S->_set, 0, num_ints * sizeof(unsigned int));

  return S;
}

/* Free memory of a set */
void set_free(Set *S)
{
  if (S != NULL) {
    if (S->_set) free(S->_set);
    free(S);
  }
}

/* Print out a set as bit vector */
void set_print(FILE *fp, Set *S)
{
  int i;
  fprintf(fp, "%d %d\n", set_size(S), set_nelements(S));
  for (i = 0; i < set_size(S); i++) {
    if (set_is_in(S, i)) fprintf(fp, "1 ");
	else fprintf(fp, "0 ");
    if (i % 20 == 19) fprintf(fp, "\n");
  }
  fprintf(fp, "\n");
}

/* Print out a set as the elements in the set */
void set_output(FILE *fp, Set *S)
{
  int i, num_elements=0;
  fprintf(fp, "%d %d\n", set_size(S), set_nelements(S));
  for (i = 0; i < set_size(S); i++) {
    if (set_is_in(S, i)) {
      fprintf(fp, "%d ", i);
      num_elements++;
      if (num_elements % 20 == 19) fprintf(fp, "\n");
    }
  }
  fprintf(fp, "\n");
}

/* Reset a set to empty */
void reset_set(Set *S)
{
  memset(S->_set, 0, bit_num_ints(S->_set_size)*sizeof(unsigned int));
  S->_num_elements = 0;
}

/* Copy set src to set dst 
   return 0 if successful, -1 if failed */ 
int set_copy(Set *dst, Set *src)
{
  int n, num_ints;
  n = set_size(dst);
  if (set_size(src) != n) return -1;  
  num_ints = n / (sizeof(unsigned int) * CHAR_BITS);
  if (n % (sizeof(unsigned int) * CHAR_BITS)) num_ints++;
  memcpy(dst->_set, src->_set, num_ints * sizeof(unsigned int));
  dst->_num_elements = src->_num_elements;
  return 0;
}

/* Return the intersection of two sets to Set dst
   return 0 if successful, -1 if failed */ 
int set_intersect(Set *dst, Set *src)
{
  int n, num_ints;
  n = set_size(dst);
  if (set_size(src) != n) return -1;  
  num_ints = n / (sizeof(unsigned int) * CHAR_BITS);
  if (n % (sizeof(unsigned int) * CHAR_BITS)) num_ints++;
  bit_AND(dst->_set, src->_set, num_ints * sizeof(unsigned int));
  dst->_num_elements = bit_count_ones(dst->_set, num_ints * sizeof(unsigned int)); 
  return 0;
}  

/* Return the union of two sets to set dst
   return 0 if success, -1 if failed */
int set_union(Set *dst, Set *src)
{
  int n, num_ints;
  n = set_size(dst);
  if (set_size(src) != n) return -1;  
  num_ints = n / (sizeof(unsigned int) * CHAR_BITS);
  if (n % (sizeof(unsigned int) * CHAR_BITS)) num_ints++;
  bit_OR(dst->_set, src->_set, num_ints * sizeof(unsigned int));
  dst->_num_elements = bit_count_ones(dst->_set, num_ints * sizeof(unsigned int)); 
  return 0;
}  


/**** array ****/

/* Malloc and initialize a array, returns a pointer to it */
Array *array_make(unsigned int size, int initval)
{
  Array *M;
  M = (Array *) malloc(sizeof(Array));
  if (M == NULL) { perror("malloc"); exit(-1); }
  M->_size = size;
  M->_array = (int *) malloc(size * sizeof(int));
  if (M == NULL) { perror("malloc"); exit(-1); }
  memset(M->_array, initval, size*sizeof(int));
  return M;
}

/* Free memory of a array */
void array_free(Array *M)
{
  if (M != NULL) {
    if (M->_array) free(M->_array);
    free(M);
  }
}

/* Print out a array as a list of mapped indices */
void array_print(FILE *fp, Array *M)
{
  unsigned int n, i;
  n = array_size(M);
//  fprintf(fp, "%d\n", n);
  for (i = 0; i < n; i++) {
	if (array_get(M, i))
      fprintf(fp, "%d\t%d\n", i, array_get(M, i));
  }
  return;
}



/**** Stack ****/

/* Make a new Stack, return the pointer to it */
Stack *stack_make(int size)
{
  Stack *S;
  S = (Stack *) malloc(sizeof(Stack));
  if (S == NULL) { perror("malloc"); exit(-1); }
  S->_size = size;
  S->_top = 0;
  S->_v = (void **) calloc(size, sizeof(void *));
  if (S->_v == NULL) { perror("calloc"); exit(-1); }
  return S;
}

/* Free a stack */
void stack_free(Stack *S)
{
  free(S->_v);
  free(S);
}

/* Pop up the element on the top of a stack */
void *stack_pop(Stack *S)
{
  void *v;
  (S->_top)--;
  v = S->_v[S->_top];
  S->_v[S->_top] = NULL;
  return v;
}

/* Push an element on to the top of a stack */
void stack_push(Stack *S, void *v)
{
  if (S->_top > S->_size) {
	fprintf(stderr, "Stack is full.\n");
	exit(1);
  }
  S->_v[S->_top] = v;
  (S->_top)++;
}


/**** Bit Operations ****/

/* return number of integers needed for a given number of bits n */
unsigned int bit_num_ints(int num_bits)
{
  unsigned int i;
  i = num_bits / (sizeof(unsigned int) * CHAR_BITS);
  if (num_bits % (sizeof(unsigned int) * CHAR_BITS)) i++;
  return i;
}

/* make a new Bitmap of a given number of bits */
void *bit_make(unsigned int num_bits)
{
  unsigned int *bits;
  unsigned int num_ints = bit_num_ints(num_bits);
  bits = (unsigned int *) calloc(num_ints, sizeof(unsigned int));
  if (bits == NULL) { perror("calloc"); exit(-1); }
  return ((void *)bits); 
}


/* return the intersection of two memory block of given size to dst */
void *bit_AND(void *dst, const void *src, int num_chars)
{
  int i;
  unsigned int *d, *s;
  d = (unsigned int *)dst;
  s = (unsigned int *)src;
  for (i = 0; i < num_chars/sizeof(unsigned int); i++) 
    d[i] &= s[i];
  return (void *)d;
}

/* return the union of two memory block of given size to dst */
void *bit_OR(void *dst, const void *src, int num_chars)
{
  int i;
  unsigned int *d = (unsigned int *)dst;
  unsigned int *s = (unsigned int *)src;
  for (i = 0; i < num_chars/sizeof(unsigned int); i++) 
	d[i] |= s[i];
  return (void *)d;
}

/* return the XOR of two memory block of given size to dst */
void *bit_XOR(void *dst, const void *src, int num_chars)
{
  int i;
  unsigned int *d = (unsigned int *)dst;
  unsigned int *s = (unsigned int *)src;
  for (i = 0; i < num_chars/sizeof(unsigned int); i++) 
	d[i] ^= s[i];
  return (void *)d;
}

/* return the number of 1's in a given bit string */
extern unsigned char bit_num_ones[256];
unsigned int bit_count_ones(void *bits, int num_chars)
{
  unsigned int num=0;
  unsigned char *b;
  int i;
  b = (unsigned char *)bits;
  for (i = 0; i < num_chars; i++) {
    num += bit_num_ones[b[i]];
  }
  return num;
}


/**** Queue ****/

/* Make a new Queue */
Queue *queue_make(int size)
{
  Queue *Q;
  Q = (Queue *) malloc(sizeof(Queue));
  if (Q == NULL) { perror("make queue"); exit(-1); }
  Q->_size = size;
  Q->_head = 0;
  Q->_tail = 0;
  Q->_nelements = 0;
  Q->_v = (void **) calloc(size, sizeof(void *));
  if (Q->_v == NULL) { perror("make queue"); exit(-1); }
  return Q;
}

/* Free a Queue */
void queue_free(Queue *Q)
{
  if (Q == NULL) return;
  if (Q->_v) free(Q->_v);
  free(Q);
  return;
}

/* Increase the size of queue */
void queue_expand(Queue *Q, int size)
{
  void **old;
  old = Q->_v;
  Q->_v = (void **) calloc(Q->_size+size, sizeof(void *));
  if (Q->_v == NULL) { perror("expand queue"); exit(-1); }
  memcpy(Q->_v, old, Q->_size*sizeof(void *));
  free(old);
  Q->_head = 0;
  Q->_tail = Q->_size;
  Q->_size += size;
}

/* Add an element to the tail of a queue */
void queue_enqueue(Queue *Q, void *e)
{
  if (Q->_nelements == Q->_size) {
	queue_expand(Q, Q->_size);
  }
  Q->_v[Q->_tail] = e;
  Q->_tail = (Q->_tail + 1) % Q->_size;
  Q->_nelements++;
  return;
}

/* Delete and return an element from the head of a queue */
void *queue_dequeue(Queue *Q)
{
  void *e;
  if (Q->_nelements == 0) return NULL;
  e = Q->_v[Q->_head];
  Q->_head = (Q->_head + 1) % Q->_size;
  Q->_nelements--;
  return e;
}


/**** Matrix of Integers ****/

/* Make a new Matrix */
Matrix *matrix_make(int nrow, int ncol)
{
  Matrix *M;
  int i;
  M = (Matrix *) malloc(sizeof(Matrix));
  if (M == NULL) { perror("malloc"); exit(-1); }
  M->_nrow = nrow;
  M->_ncol = ncol;
  M->_element = (int **) malloc(nrow * sizeof(int *));
  if (M->_element == NULL) { perror("malloc"); }
  M->_element[0] = (int *) calloc(nrow * ncol, sizeof(int));
  for (i = 1; i < nrow; i++) {
    M->_element[i] = M->_element[0] + i * ncol;
  }
  return M;
}

/* Free a Matrix */
void matrix_free(Matrix *M)
{
 if (M) {
   if (M->_element) {
     if (M->_element[0]) free(M->_element[0]);
     free(M->_element);
   }
   free(M);
 }
}

/* Print out a Matrix */
void matrix_printf(FILE *fp, Matrix *M)
{
  int i, j;
  for (i = 0; i < M->_nrow; i++) {
    for (j = 0; j < M->_ncol; j++) {
      fprintf(fp, "%d ", M->_element[i][j]);
    }
    fprintf(fp, "\n");
  }
}


