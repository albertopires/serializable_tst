// Copyright (C) 2023 Alberto pires de Oliveira Neto
// Distributed under MIT license, or public domain if desired and
// recognized in your jurisdiction.
// See file LICENSE for detail or copy at https://github.com/albertopires/serializable_tst/blob/main/LICENSE

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "ctst.h"

void tst_handler(const char *key, int key_len, NODE* node, void *param);

int main(int argc, char *argv[])
{
        NODE *root;

	if (argc < 2) {
		printf("%s <serialized_file.ser> <search_key>\n", argv[0]);
		exit(1);
	}

	root = NULL;
	deserialize(&root, argv[1]);

	printf("Traverse\n");
	traverse_data_nodes(root, tst_handler, NULL);
	printf("------------------------\n");

	printf("Search: <%s>\n", argv[2]);
        if (argc == 3) {
                NODE *node = tst_get(&root, argv[2]);
                printf("find match: %d\n", node != NULL);
                if (node) {
                        printf("Node char : %c\n", node->c);
			if (node->data.buffer) {
				printf("Node data: <%s>\n", node->data.buffer);
			}
                        if (node->mid) {
                                printf("Node next : %c\n", node->mid->c);
                        } else {
                                printf("Node next : NULL\n");
                        }
                }
        }


	exit(0);
}
void tst_handler(const char *key, int key_len, NODE* node, void *param)
{
        char print_key[key_len+1];
        strncpy(print_key, key, key_len);
        print_key[key_len] = 0;
        printf("-> Key: <%s>/%d, data: %s\n", print_key, key_len, node->data.buffer);
}
