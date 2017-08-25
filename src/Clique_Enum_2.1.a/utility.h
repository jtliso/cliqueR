/* APIs of utility for undirected graphs */
/* Graph Theory Team, Computer Science Department */ 
/* University of Tennessee, Knoxville */
/* Yun Zhang, yzhang@cs.utk.edu, January 20, 2005 */

#ifndef __UTILITY_H
#define __UTILITY_H


/** Macros **/

#define CHAR_BITS  8  /* number of bits in char */

#define XBM  31
#define RSV  5

#define IS_SET(vector, bit)   (vector[(bit) >> RSV] & 1 << ((bit) & XBM))
#define SET_BIT(vector, bit)  (vector[(bit) >> RSV] |= 1 << ((bit) & XBM))
#define DEL_BIT(vector, bit)  (vector[(bit) >> RSV] &= ~(1 << ((bit) & XBM)))

#define SET_VALUE(vector, index, value)  (vector[index] = value)
#define GET_VALUE(vector, index)         (vector[index])


typedef unsigned long  u64;
typedef unsigned int   u32;
typedef unsigned short u16;
typedef unsigned char  u8;


/**** Timer function ****/

/* Returns current time in seconds as a double value
   and the precision can be as accurate as microseconds
   (10^-6 second) 
 */
double get_cur_time();


/** Data structure **/

/* Data structure of a Set of binary elements */
typedef struct set_t {
  unsigned int _set_size;     /* total number of vertices */
  unsigned int _num_elements; /* number of vertices in the set */
  unsigned int *_set;         /* bit-based vector: 1 - in set */
} Set;

/** Macros for Set **/

/* returns the size of set */
#define set_size(s)  (s->_set_size)

/* returns the number of elements in set */
#define set_nelements(s)  (s->_num_elements)

/* set the number of elements in set */
#define set_nelements_set(s, n)  (s->_num_elements = n)

/* check to see if an element in set or not */
#define set_is_in(s, u)  (IS_SET(s->_set, u))

/* add an element to set */
#define set_add(s, u) {\
  if (!IS_SET(s->_set, u)) { SET_BIT(s->_set, u); s->_num_elements++; }\
}

/* delete an element from set */
#define set_delete(s, u) {\
  if (IS_SET(s->_set, u)) { DEL_BIT(s->_set, u); s->_num_elements--; }\
}

/** Function prototypes for Set **/

/* Malloc a set, initialize it and returns a pointer to it */
Set *set_make(unsigned int n);

/* Free the memory of a set */
void set_free(Set *S);

/* Print out a set as 0/1 vector */
void set_print(FILE *fp, Set *S);

/* Print out a set as a set */
void set_output(FILE *fp, Set *S);

/* Reset a set to empty */
void set_reset(Set *S);

/* Copy set src to set dst 
   return 0 if successful, -1 if failed */ 
int set_copy(Set *dst, Set *src);

/* Return the intersection of two sets to Set dst
   return 0 if successful, -1 if failed */
int set_intersect(Set *dst, Set *src);
	
/* Return the union of two sets to set dst
   return 0 if success, -1 if failed */
int set_union(Set *dst, Set *src);



/**** Array of Integer ****/

/* Data structure of an Array */
typedef struct array_t {
  unsigned int _size;
  int *_array;
} Array;


/** Macros for Array **/

/* returns the size of array */
#define array_size(m)  (m->_size)

/* set the mapped value of vertex u */
#define array_set(m, u, value)  (SET_VALUE(m->_array, u, value))

/* get the mapped value of vertex u */
#define array_get(m, u)  (GET_VALUE(m->_array, u))

/** Function prototypes for array **/

/* Malloc a array, initialize it and returns a pointer to it */
Array *array_make(unsigned int size, int initval);

/* Free the memory of a set */
void array_free(Array *M);

/* Print out a array as array list */
void array_print(FILE *fp, Array *M);



/**** Stack ****/

/* Data structure of a Stack */
typedef struct stack_t {
  void **_v;
  int _top;
  int _size;
} Stack;


/** Macros for Stack **/

/* return the total size of a stack */
#define stack_size(s) (s->_size)

/* return the number of elements in a stack */
#define stack_length(s) (s->_top)

/* return whether a stack is empty or not */
#define stack_empty(s) (!(s->_top))

/* return the element on the top of a stack */
#define stack_top(s) (s->_v[s->_top-1])

/* delete the first k elements from the top of a stack */
#define stack_pop_k(s, k) {\
  void *my_v; int my_i; \
  for (my_i = 0; my_i < k; my_i++) {\
    my_v = stack_pop(s);\
    free(my_v); }\
}

/** Function prototypes for Stack */

/* Make a new Stack of given size */
Stack *stack_make(int size);

/* Free a stack */
void stack_free(Stack *S);

/* Pop up the element on the top of a stack */
void *stack_pop(Stack *S);

/* Push an element on to a stack's top */
void stack_push(Stack *S, void *v);



/**** Bit operations ****/

/* malloc a new Bitmap of given number of bits needed */
void *bit_make(unsigned int num_bits);

/* return the intersection of two memory block of given size to dst */
void *bit_AND(void *dst, const void *src, int num_chars);

/* return the union of two memory block of given size to dst */
void *bit_OR(void *dst, const void *src, int num_chars);

/* return the xor of two memory block of given size to dst */
void *bit_XOR(void *dst, const void *src, int num_chars);

/* return the number of 1's in a given string */
unsigned int bit_count_ones(void *string, int num_chars);

/* return number of integers needed for a given number of bits */
unsigned int bit_num_ints(int num_bits);


/**** Queue ****/

/* Data Structure a Queue */
typedef struct queue_t {
  void **_v;
  int _size;      /* total size of the queue */
  int _head;      /* position of first element */
  int _tail;      /* position of last element */
  int _nelements; /* number of elements in queue */
} Queue;


/** Macros for Queue **/

/* return the size of the queue */
#define queue_size(q)  (q->_size)

/* return the number of elements in queue */
#define queue_nelem(q)  (q->_nelements)

/* return the first element in the queue */
#define queue_head(q)  (q->_v[q->_head])

/* return whether a queue is empty */
#define queue_empty(q)  (!(q->_nelements))

/* Function prototypes for Queue */

/* Make a new Queue */
Queue *queue_make(int size);

/* Free a Queue */
void queue_free(Queue *Q);

/* Increase Queue size */
void queue_expand(Queue *Q, int size);

/* Add an element to the tail of a queue */
void queue_enqueue(Queue *Q, void *e);

/* Delete and return an element from the head of a queue */
void *queue_dequeue(Queue *Q);



/**** Matrix of Integers ****/

/* Data Structure of a Matrix */
typedef struct matrix_t {
  int **_element;
  int _nrow;
  int _ncol;
} Matrix;

/** Macros for Matrix **/

/* Return the number of rows */
#define matrix_nrow(m)  (m->_nrow)

/* Return the number of columns */
#define matrix_ncol(m)  (m->_ncol)

/* Return the value of cell m[i][j] */
#define matrix_get(m, i, j)  (m->_element[i][j])

/* Set the value of m[i][j] to v */
#define matrix_set(m, i, j, v)  { m->_element[i][j]=v; m->_element[j][i]=v; }

/* Reset the matrix to an init value v */
#define matrix_reset(m, v) (memset(m->_element[0], v, m->_nrow*m->_ncol*sizeof(int)))

/** Function prototypes for Matrix **/

/* Make a new Matrix */
Matrix *matrix_make(int nrow, int ncol);

/* Free a Matrix */
void matrix_free(Matrix *M);

/* Print out a Matrix to a file */
void matrix_printf(FILE *fp, Matrix *M);



#endif  /* __UTILITY_H */

