#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <syrup/array-list.h>

struct sy_array_s {
  void **ptr;
  size_t len;
  size_t allocs;
  sy_array_comparator_fn comp;
};

#define SY_ARRAY_LIST_BLOCK_SIZE 10

static bool alloc_atleast(sy_array_t *str, size_t len) {
  if (str->allocs > len)
    return true;
  int i = ceil((double)len / (double)SY_ARRAY_LIST_BLOCK_SIZE);
  int nsize = i * SY_ARRAY_LIST_BLOCK_SIZE;
  void **data = realloc(str->ptr, sizeof(void **) * nsize);

  if (!data) {
    return false;
  }
  str->ptr = data;
  str->allocs = nsize;

  return true;
}

sy_array_t *sy_array_new(sy_array_comparator_fn fn) {
  sy_array_t *a = malloc(sizeof(sy_array_t));
  a->len = 0;
  a->allocs = SY_ARRAY_LIST_BLOCK_SIZE;
  a->comp = fn;
  a->ptr = malloc(sizeof(void **) * a->allocs);
  return a;
}

void sy_array_foreach(sy_array_t *a, sy_array_iterate_fn fn) {
  for (size_t i = 0; i < a->len; i++) {
    fn(a->ptr[i], i);
  }
}

void sy_array_free(sy_array_t *a) {
  if (!a)
    return;
  free(a->ptr);
  free(a);
}

void sy_array_append(sy_array_t *a, void *item) {
  if (!alloc_atleast(a, a->len + 1))
    return;
  a->ptr[a->len++] = item;
}

int sy_array_indexof(sy_array_t *a, void *item) {
  for (size_t i = 0; i < a->len; i++) {
    if (a->comp(a->ptr[i], item) == 0)
      return i;
  }
  return -1;
}
bool sy_array_remove_index(sy_array_t *a, int i) {
  if (i >= a->len)
    return false;
  else if (i < a->len - 1)
    memcpy(a->ptr + i, a->ptr + i + 1, a->len - i);
  a->len--;
  return true;
}

void *sy_array_get(sy_array_t *a, int index) {
  if (index >= a->len) {
    return NULL;
  }
  return a->ptr[index];
}

size_t sy_array_len(sy_array_t *a) { return a->len; }

void sy_array_sort(sy_array_t *a) {
  // Bubble sort for now
  void *tmp;
  size_t i = 0, j;
  for (; i < a->len; i++) {
    for (j = 0; j < a->len; j++) {
      if (a->comp(a->ptr[i], a->ptr[j]) < 0) {
        tmp = a->ptr[i];
        a->ptr[i] = a->ptr[j];
        a->ptr[j] = tmp;
      }
    }
  }
}

void **sy_array_raw(sy_array_t *a) { return a->ptr; }