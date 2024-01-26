// Copyright (C) 2023 Alberto pires de Oliveira Neto
// Distributed under MIT license, or public domain if desired and
// recognized in your jurisdiction.
// See file LICENSE for detail or copy at https://github.com/albertopires/serializable_tst/blob/master/LICENSE

#ifndef CTST_VAL_H_
#define CTST_VAL_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>

#include "mem_alloc.h"

#define CTST_OK    0
#define CTST_ERROR 1

typedef struct node_st {
	struct node_st *mid;
	struct node_st *left;
	struct node_st *right;
	struct dyn_buffer_st data;
	char c;
} NODE;

typedef void (PROC_HANDLER)(const char*, int, NODE*, void *); // (buff, strlen(found key), node, param)

void print_node_count(void);
NODE *tst_put(NODE **root, const char *str);
NODE *tst_put_max(NODE **root, const char *str, int ks);
NODE *tst_get(NODE **root, const char *str);
void serialize(NODE *root, const char *file_name);
void delete_tree(NODE **node);
void traverse_nodes(NODE *root, PROC_HANDLER proc_handler, void *param, bool data);
void debug(NODE *root);
int deserialize(NODE **root, const char *file_name);

#endif
