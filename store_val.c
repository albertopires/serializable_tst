// Copyright (C) 2023 Alberto pires de Oliveira Neto
// Distributed under MIT license, or public domain if desired and
// recognized in your jurisdiction.
// See file LICENSE for detail or copy at https://github.com/albertopires/serializable_tst/blob/master/LICENSE

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "ctst.h"

static int read_line(FILE *fp, int size, void *line, void *param);
void tst_handler(const char *key, int key_len, NODE* node, void *param);
void find_node_and_add(NODE *root, const char *key, const char *str);
void show_node_data(NODE *root, const char *key);

int main(int argc, char *argv[])
{
        FILE *fp;
        NODE *root = NULL;
        char buffer[2048];

        memset(buffer, 0, sizeof(buffer));

	if (argc < 2) {
		printf("%s <file.txt> <search_key>\n", argv[0]);
		exit(1);
	}

	fp = fopen(argv[1], "r");
	if (fp == NULL) {
		perror("fopen");
		exit(1);
	}

	NODE *n;
	while (read_line(fp, 2048, &buffer, NULL) == 0)  {
		n = tst_put(&root, buffer);
		if (!n) {
			printf("Failed to store node\n");
			abort();
		}
	}


	fclose(fp);

	find_node_and_add(root, "passwd", "Stores passwords");
	find_node_and_add(root, "perl", "Programming language");


	show_node_data(root, "passwd");
	show_node_data(root, "perl");

	printf("Traverse\n");
	traverse_data_nodes(root, tst_handler, NULL);

	serialize(root, "data.ser");

	exit(0);
}

void find_node_and_add(NODE *root, const char *key, const char *str)
{
	NODE *node = tst_get(&root, key);
	if (node) {
		if (!node->data.buffer) {
			allocate_buffer(&node->data, 500);
			node->data.buf_size = sprintf(node->data.buffer, "%s", str);
		}
	} else {
		printf("Node <%s> not found\n", key);
	}
}

void show_node_data(NODE *root, const char *key)
{
	NODE *node = tst_get(&root, key);
	if (node) {
		if (node->data.buffer) {
			printf("Node data: Key<%s>/<%s>\n", key, node->data.buffer);
		}
	} else {
		printf("Node <%s> not found\n", key);
	}
}

void tst_handler(const char *key, int key_len, NODE* node, void *param)
{
	char print_key[key_len+1];
	strncpy(print_key, key, key_len);
	print_key[key_len] = 0;
        printf("-> Key: <%s>/%d, data: %s\n", print_key, key_len, node->data.buffer);
}

static int read_line(FILE *fp, int size, void *line, void *param)
{
        size_t s = size, br;
        char *l = line;

        if ((br = getline(&l, &s, fp)) == -1) {
                printf("End of file\n");
                return 1;
        }
	if (l[br-1] == 0x0a) {
		l[br-1] = 0;
	}

        return 0;
}
