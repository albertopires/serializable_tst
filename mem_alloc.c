// Copyright (C) 2023 Alberto pires de Oliveira Neto
// Distributed under MIT license, or public domain if desired and
// recognized in your jurisdiction.
// See file LICENSE for detail or copy at https://github.com/albertopires/serializable_tst/blob/main/LICENSE

#include "mem_alloc.h"

/* Standard heap allocation */
void allocate_buffer(struct dyn_buffer_st *dyn_buffer, int size)
{
	dyn_buffer->buffer = (char*) malloc(size);
	if (!dyn_buffer->buffer) {
		printf("allocate_buffer: Malloc error - line: %d\n", __LINE__);
		sleep(120);
		abort();
	}
	dyn_buffer->capacity = size;
	dyn_buffer->buf_size = 0;
}

void reallocate_buffer(struct dyn_buffer_st *dyn_buffer, int size)
{
	if (size <= dyn_buffer->capacity) return;

	printf("reallocate buffer - old: %d, new: %d\n",
			dyn_buffer->capacity,size);
	dyn_buffer->buffer = (char*) realloc(dyn_buffer->buffer, size);
	if (dyn_buffer->buffer == NULL) {
		printf("reallocate_buffer: Realloc error - line: %d\n", __LINE__);
		abort();
	}
	dyn_buffer->capacity = size;
}

void free_buffer(struct dyn_buffer_st *dyn_buffer)
{
	if (dyn_buffer->buffer) free (dyn_buffer->buffer);
	memset(dyn_buffer, 0, sizeof(struct dyn_buffer_st));
}
