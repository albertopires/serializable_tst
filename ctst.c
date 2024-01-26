// Copyright (C) 2023 Alberto pires de Oliveira Neto
// Distributed under MIT license, or public domain if desired and
// recognized in your jurisdiction.
// See file LICENSE for detail or copy at https://github.com/albertopires/serializable_tst/blob/master/LICENSE

#include "ctst.h"

static char MARKER = 0x1;
static char ENDSTR = 0x2;
static char DATAMK = 0x3;
static int node_count = 0;

void print_node_count(void)
{
	printf("Node count: %d\n", node_count);
}

static bool is_leaf_node(NODE *node)
{
	return !(node->left) && !(node->mid) && !(node->right);
}

static NODE *create_node(void)
{
	struct node_st *node;
	node = (struct node_st*) malloc(sizeof(struct node_st));
	if (node) {
		node_count++;
		memset(node, 0, sizeof(NODE));
		return node;
	}
	printf("failed to allocate node\n");
	exit(1);
}

static void delete_leaf_node(NODE *node)
{
	if (!is_leaf_node(node)) abort();
	node_count--;
	if (node->data.buffer) free_buffer(&node->data);
	free(node);
}

static NODE *tst_put_r(NODE **node, const char *str, int d, int l)
{
	if (d == l) return *node;
	if (str[d] == 0x0a) return *node;

	if (*node == NULL) {
		*node = create_node();
		(*node)->c = str[d];
	}

	if      (str[d] < (*node)->c) return tst_put_r(&((*node)->left), str, d, l);
	else if (str[d] > (*node)->c) return tst_put_r(&((*node)->right), str, d, l);
	else if (d < l-1) return tst_put_r(&((*node)->mid), str, d+1, l);

	return *node;
}

NODE *tst_put(NODE **root, const char *str)
{
	return tst_put_r(root, str, 0, strlen(str));
}

NODE *tst_put_max(NODE **root, const char *str, int ks)
{
	char key[ks+1];

	memset(key, 0, ks+1);
	strncpy(key, str, ks);

	return tst_put_r(root, key, 0, strlen(key));
}

//
static NODE* tst_get_r(NODE **node, const char *str, int d, int l)
{
	if (d == l) return NULL;
	if (*node == NULL) return NULL;
	//printf("<%c>\n", (*node)->c);

	if      (str[d] < (*node)->c) return tst_get_r(&((*node)->left), str, d, l);
	else if (str[d] > (*node)->c) return tst_get_r(&((*node)->right), str, d, l);
	else if (d < l-1)             return tst_get_r(&((*node)->mid), str, d+1, l);
	else                          return (*node);
}

NODE *tst_get(NODE **root, const char *str)
{
	return tst_get_r(root, str, 0, strlen(str));
}

//
static void serialize_file(NODE *root, FILE *fp)
{
	while (root) {
		fprintf(fp, "%c", root->c);
		if (root->data.buffer) { // this node has data
			fprintf(fp, "%c", DATAMK);
			fprintf(fp, "%x ", *((int32_t*)&root->data.buf_size));
			fwrite(root->data.buffer, root->data.buf_size, 1, fp);
		}
		if (root->left) {
			fprintf(fp, "%c", MARKER);
			serialize_file(root->left, fp);
			fprintf(fp, "%c", ENDSTR);
		}
		if (root->right) {
			fprintf(fp, "%c", MARKER);
			serialize_file(root->right, fp);
			fprintf(fp, "%c", ENDSTR);
		}
		root = root->mid;
	}
}

void serialize(NODE *root, const char *file_name)
{
	FILE *fp;

	remove(file_name);
	fp = fopen(file_name, "wb");
	if (fp == NULL) {
		perror("fopen (serialize)");
		exit(1);
	}

	serialize_file(root, fp);

	fclose(fp);
}

static int fpeek(FILE *stream)
{
	int c;

	c = fgetc(stream);
	ungetc(c, stream);

	return c;
}

// De-Serialize
static void deserialize_file(NODE **root, FILE *fp)
{
	char c,next;

	if (fscanf(fp, "%c", &c) == EOF) return;
	if (c == ENDSTR) return;
	if (*root == NULL) {
		*root = create_node();
		(*root)->c = c;
		do {
			next = fpeek(fp);
			if (next == DATAMK) {
				if(fscanf(fp, "%c", &c) == EOF) return; // flush marker
				int ic;
				fscanf(fp, "%x ", &ic);
				allocate_buffer(&(*root)->data, ic+1);
				(*root)->data.buf_size = ic;
				(*root)->data.buffer[ic] = 0;
				fread((*root)->data.buffer, ic, sizeof(char), fp);
				next = fpeek(fp);
			}
			if (next == MARKER) {
				if(fscanf(fp, "%c", &c) == EOF) return; // flush marker
				c = fpeek(fp);
				if (c < (*root)->c)
					deserialize_file(&((*root)->left), fp);
				if (c > (*root)->c)
					deserialize_file(&((*root)->right), fp);
			}
		} while (next == MARKER);
		deserialize_file(&((*root)->mid), fp);
	}
}
int deserialize(NODE **root, const char *file_name)
{
	FILE *fp;

	fp = fopen(file_name, "rb");
	if (fp == NULL) {
		perror("fopen - dserialize");
		return CTST_ERROR;
	}

	deserialize_file(root, fp);

	fclose(fp);
	return CTST_OK;
}

void delete_tree(NODE **node)
{
	if (*node == NULL) return;
	delete_tree(&(*node)->left);
	delete_tree(&(*node)->mid);
	delete_tree(&(*node)->right);
	if (is_leaf_node(*node)) {
		delete_leaf_node(*node);
		*node = NULL;
		return;
	}
}

static void traverse_nodes_r(NODE *node, PROC_HANDLER proc_handler, void *param, bool data, char *buf, int i)
{
	if (node) {
		traverse_nodes_r(node->left, proc_handler, param, data, buf, i);

		buf[i] = node->c;
		if (data && node->data.buffer) {
			// Call proc_handler only on data nodes
			proc_handler(buf, i+1, node, param);
		} else if (!data && is_leaf_node(node)) {
			proc_handler(buf, i+1, node, param);
		}

		traverse_nodes_r(node->mid, proc_handler, param, data, buf, i+1);
		traverse_nodes_r(node->right, proc_handler, param, data, buf, i);
	}
}

// data: true  - call handler only data leaf nodes
//       false - call handler on all leaf nodes
void traverse_nodes(NODE *root, PROC_HANDLER proc_handler, void *param, bool data)
{
	char buffer[512];

	memset(buffer, 0, sizeof(buffer));
	traverse_nodes_r(root, proc_handler, param, data, buffer, 0);
}

void debug(NODE *root)
{
	NODE *n;

	printf("<%c>", root->c);
	n = root->mid;
	while (n) {
		printf("<%c>", n->c);
		n = n->mid;
	}
	printf("\n");
}

