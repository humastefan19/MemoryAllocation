#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <assert.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
//specificatiile blocului de memorie 
struct specificatii_block {
  size_t size;
  struct specificatii_block *next;
  int free;
};
#define SPECIFICATII_SIZE sizeof(struct specificatii_block)

//lista in care se aloca toate datele
void *lista_date = NULL;

struct specificatii_block *gaseste_zona_libera(struct specificatii_block **last, size_t size) {
  struct specificatii_block *current = lista_date;
  while (current && !(current->free && current->size >= size)) {
    *last = current;
    current = current->next;
  }
  return current;
}

struct specificatii_block *ask_space_heap(struct specificatii_block* last, size_t size) {
  struct specificatii_block *block;
  block = sbrk(0);
  void *ask = sbrk(size + SPECIFICATII_SIZE);
  assert((void*)block == ask);
  if (ask == (void*) -1) {
    return NULL; // daca sbrk nu a putut sa aloce
  }

  if (last) { 
    last->next = block;
  }
  block->size = size;
  block->next = NULL;
  block->free = 0;
  return block;
}

void *myalloc(size_t size) {
  struct specificatii_block *block;
  

  if (size <= 0) {
    return NULL;
  }

  if (!lista_date) { // pentru prima punere in lista de date
    block = ask_space_heap(NULL, size);
    if (!block) {
      return NULL;
    }
    lista_date = block;
  } else {
    struct specificatii_block *last = lista_date;
    block = gaseste_zona_libera(&last, size);
    if (!block) { // cautare block liber
      block = ask_space_heap(last, size);
      if (!block) {
        return NULL;
      }
    } else {      //gasit blcok liber si face free 0
      block->free = 0;
      
    }
  }

  return(block+1);
}

struct specificatii_block *get_block_ptr(void *ptr) {
  return (struct specificatii_block*)ptr - 1;
}


void myfree(void *ptr) {
  if (!ptr) {
    return;
  }

  // functia pentru eliberarea de memorie
  struct specificatii_block* block_ptr = get_block_ptr(ptr);
  assert(block_ptr->free == 0);
  block_ptr->free = 1;
}

void *myRealloc(void *ptr, size_t size) {
  if (!ptr) {
    //daca ptr e null atunci vom apela myalloc
    return myalloc(size);
  }

  struct specificatii_block* block_ptr = get_block_ptr(ptr);
  if (block_ptr->size >= size) {
    // daca nu se da spatul cerut mai mare decat cel initial se returneaza pointerul vechi
    return ptr;
  }

  // copiem vechiul poniter intr-unul nou
  void *new_ptr;
  new_ptr = myalloc(size);
  if (!new_ptr) {
    return NULL; 
  }
  memcpy(new_ptr, ptr, block_ptr->size);
  myfree(ptr);
  return new_ptr;
}
