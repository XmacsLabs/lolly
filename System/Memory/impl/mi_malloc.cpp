
/******************************************************************************
 * MODULE     : Fast memory allocation using mimalloc
 * DESCRIPTION:
 * COPYRIGHT  : (C) 1999  Joris van der Hoeven
 *******************************************************************************
 * This software falls under the GNU general public license version 3 or later.
 * It comes WITHOUT ANY WARRANTY WHATSOEVER. For details, see the file LICENSE
 * in the root directory or <http://www.gnu.org/licenses/gpl-3.0.html>.
 ******************************************************************************/

#include "fast_alloc.hpp"
#include "assert.h"
#include "mimalloc.h"

int mem_used ();

/*****************************************************************************/
// General purpose fast allocation routines
/*****************************************************************************/

void*
safe_malloc (size_t sz) {
  void* ptr= NULL;
  if (sz < MI_SMALL_SIZE_MAX)
  {
    ptr = mi_malloc_small (sz);
  }else{
    
    ptr = mi_malloc (sz);
  }
  
  if (ptr == NULL) {
    cerr << "Fatal error: out of memory\n";
    abort ();
  }
  return ptr;
}

void*
enlarge_malloc (size_t sz) {
  return NULL;
}

void*
fast_alloc (size_t sz) {
  return safe_malloc (sz);
}

void
fast_free (void* ptr, size_t sz) {
  mi_free (ptr);
}

void*
fast_new (size_t s) {
  return safe_malloc (s);
}

void
fast_delete (void* ptr) {
  mi_free (ptr);
}

/******************************************************************************
 * Statistics
 ******************************************************************************/

bool
visit_mem (const mi_heap_t* heap, const mi_heap_area_t* heapinfo, void* block,
           size_t blocksize, void* arg) {
  int* count= (int*) arg;
  assert(heapinfo != NULL);
  *count+= heapinfo->used;
  return true;
}

int
mem_used () {
  mi_heap_t* heap = mi_heap_get_default ();
  int        count= 0;
  mi_heap_visit_blocks (heap, false, visit_mem, &count);
  return count;
}

void
mem_info () {
  cout << "\n---------------- memory statistics ----------------\n";
}

/******************************************************************************
 * Redefine standard new and delete
 ******************************************************************************/

#if defined(X11TEXMACS) && (!defined(NO_FAST_ALLOC))

void*
operator new (size_t s) {
  void* ptr;
  s= (s + WORD_LENGTH + WORD_LENGTH_INC) & WORD_MASK;
  if (s < MAX_FAST) {
    ptr= alloc_ptr (s);
    if (ptr == NULL) ptr= enlarge_malloc (s);
    else alloc_ptr (s)= ind (ptr);
  }
  else {
    ptr= safe_malloc (s);
    large_uses+= s;
  }
  *((size_t*) ptr)= s;
  return (void*) (((char*) ptr) + WORD_LENGTH);
}

void
operator delete (void* ptr) {
  ptr     = (void*) (((char*) ptr) - WORD_LENGTH);
  size_t s= *((size_t*) ptr);
  if (s < MAX_FAST) {
    ind (ptr)    = alloc_ptr (s);
    alloc_ptr (s)= ptr;
  }
  else {
    free (ptr);
    large_uses-= s;
  }
}

void*
operator new[] (size_t s) {
  void* ptr;
  s= (s + WORD_LENGTH + WORD_LENGTH_INC) & WORD_MASK;
  if (s < MAX_FAST) {
    ptr= alloc_ptr (s);
    if (ptr == NULL) ptr= enlarge_malloc (s);
    else alloc_ptr (s)= ind (ptr);
  }
  else {
    ptr= safe_malloc (s);
    large_uses+= s;
  }
  *((size_t*) ptr)= s;
  return (void*) (((char*) ptr) + WORD_LENGTH);
}

void
operator delete[] (void* ptr) {
  ptr     = (void*) (((char*) ptr) - WORD_LENGTH);
  size_t s= *((size_t*) ptr);
  if (s < MAX_FAST) {
    ind (ptr)    = alloc_ptr (s);
    alloc_ptr (s)= ptr;
  }
  else {
    free (ptr);
    large_uses-= s;
  }
}

#endif // defined(X11TEXMACS) && (!defined(NO_FAST_ALLOC))
