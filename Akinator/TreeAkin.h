#pragma once
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define _SUPERDEBUG_ 1

struct tree_ak
{
	int phrase;
	struct tree_ak * yes_left;
	struct tree_ak * no_right;
};

typedef struct tree_ak tree_akin;

tree_akin * read_tree(FILE * input);
void tree_akin_dtor(tree_akin * destruct_this);
int tree_akin_is_ok(tree_akin * check_this);
int tree_akin_dump(tree_akin * dump_this, int deep);
int tree_akin_print(tree_akin * print_this, int deep, char ** phrases);