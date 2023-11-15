// Copyright (C) 2023 Alberto pires de Oliveira Neto
// Distributed under MIT license, or public domain if desired and
// recognized in your jurisdiction.
// See file LICENSE for detail or copy at https://github.com/albertopires/serializable_tst/blob/main/LICENSE

#ifndef CTST_VAL_H_
#define CTST_VAL_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "mem_alloc.h"

typedef struct node_st {
	struct node_st *mid;
	struct node_st *left;
	struct node_st *right;
	struct dyn_buffer_st data;
	char c;
} NODE;

typedef void (PROC_HANDLER)(const char*, int, NODE*, void *); // (buff, strlen(found key), node, param)

NODE *create_node(void);
NODE *tst_put(NODE **root, const char *str);
NODE *tst_put_max(NODE **root, const char *str, int ks);
NODE *tst_get(NODE **root, const char *str);
void serialize(NODE *root, const char *file_name);
void traverse(NODE *root);
void traverse_data_nodes(NODE *root, PROC_HANDLER proc_handler, void *param);
void debug(NODE *root);
void deserialize(NODE **root, const char *file_name);

#endif
