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

static bool alloc_atleast(sy_array_t *str, size_t len) {
  if (str->allocs > len)
    return true;
  int i = ceil((double)len / (double)64);
  int nsize = i * 64;
  unsigned char *data = realloc(str->ptr, sizeof(void **) * nsize);

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
  a->allocs = 1;
  a->comp = fn;
  a->ptr = malloc(sizeof(void **) * a->allocs);
  return a;
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

size_t sy_array_len(sy_array_t *a) { return a->len; }