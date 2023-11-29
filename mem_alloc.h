// Copyright (C) 2023 Alberto pires de Oliveira Neto
// Distributed under MIT license, or public domain if desired and
// recognized in your jurisdiction.
// See file LICENSE for detail or copy at https://github.com/albertopires/serializable_tst/blob/master/LICENSE

#ifndef MEM_ALLOC_H_
#define MEM_ALLOC_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct dyn_buffer_st {
	int capacity; // Real buffer size (malloc/realloc)
	int buf_size; // data size (eg. read/pread)
	char *buffer;
};

void allocate_buffer(struct dyn_buffer_st *dyn_buffer, int size);
void reallocate_buffer(struct dyn_buffer_st *dyn_buffer, int size);
void free_buffer(struct dyn_buffer_st *dyn_buffer);

#endif
