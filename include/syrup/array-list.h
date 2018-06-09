#pragma once
#include <stdbool.h>
#include <stddef.h>

typedef struct sy_array_s sy_array_t;

// typedef struct sy_array_item_s {
//   void *data;

// } sy_array_item_t;

typedef int (*sy_array_comparator_fn)(void *lh, void *rh);

sy_array_t *sy_array_new(sy_array_comparator_fn fn);

void sy_array_append(sy_array_t *, void *);
int sy_array_indexof(sy_array_t *, void *);
bool sy_array_remove_index(sy_array_t *, int i);
size_t sy_array_len(sy_array_t *);