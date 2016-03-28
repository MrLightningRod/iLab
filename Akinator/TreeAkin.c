#include "TreeAkin.h"
#define ASSERT_PRINTF(x) assert((x > 0) || (!"All gone bad with printf, nothing to do here"))

const int _DUMP_INDENT_ = 4;

int tree_akin_is_ok(tree_akin * check_this)
{
	if (check_this == NULL) return 0;
	int result = 1;
	if (check_this->phrase < 0) return 0;
	if ((check_this->yes_left != NULL) && (check_this->no_right != NULL))
	{
		result = tree_akin_is_ok(check_this->yes_left);
		if (!result) return 0;
		result = tree_akin_is_ok(check_this->no_right);
		return result;
	}
	if ((check_this->yes_left == NULL) && (check_this->no_right == NULL)) return result;
	return 0;
}

int tree_akin_dump(tree_akin * dump_this, int deep)
{
	assert(deep >= 0);
	if (dump_this == NULL) return 1;
	int i = 0;
	int err_printf = 0;


	for (i = 0; i < deep * _DUMP_INDENT_; i++)
	{
		err_printf = printf("-");
		ASSERT_PRINTF(err_printf);
	}
	err_printf = printf("CURRENT: %d\n", (int)dump_this);
	ASSERT_PRINTF(err_printf);

	for (i = 0; i < deep * _DUMP_INDENT_; i++)
	{
		err_printf = printf("-");
		ASSERT_PRINTF(err_printf);
	}
	err_printf = printf("PHRASE: %d\n", dump_this->phrase);
	ASSERT_PRINTF(err_printf);


	for (i = 0; i < deep * _DUMP_INDENT_; i++)
	{
		err_printf = printf("-");
		ASSERT_PRINTF(err_printf);
	}
	err_printf = printf("YES LEFT: %d\n", (int)dump_this->yes_left);
	ASSERT_PRINTF(err_printf);


	for (i = 0; i < deep * _DUMP_INDENT_; i++)
	{
		err_printf = printf("-");
		ASSERT_PRINTF(err_printf);
	}
	err_printf = printf("NO RIGHT: %d\n", (int)dump_this->no_right);
	ASSERT_PRINTF(err_printf);

	for (i = 0; i < deep * _DUMP_INDENT_; i++)
	{
		err_printf = printf(" ");
		ASSERT_PRINTF(err_printf);
	}
	err_printf = printf("\n");
	ASSERT_PRINTF(err_printf);
	tree_akin_dump(dump_this->yes_left, deep + 1);
	tree_akin_dump(dump_this->no_right, deep + 1);
	return 1;
}

void tree_akin_dtor(tree_akin * destruct_this)
{
	if (destruct_this == NULL) return;
	destruct_this->no_right = NULL;
	destruct_this->yes_left = NULL;
	destruct_this->phrase = 0;
	tree_akin_dtor(destruct_this->yes_left);
	tree_akin_dtor(destruct_this->no_right);
	free(destruct_this);
}

int tree_akin_print(tree_akin * print_this, int deep, char ** phrases)
{
	assert(phrases);
	assert(deep >= 0);
	if (print_this == NULL) return 1;
	int i = 0;
	int err_printf = 0;


	for (i = 0; i < deep * _DUMP_INDENT_; i++)
	{
		err_printf = printf("-");
		ASSERT_PRINTF(err_printf);
	}
	err_printf = printf("PHRASE: %s\n", phrases[print_this->phrase]);
	ASSERT_PRINTF(err_printf);

	for (i = 0; i < deep * _DUMP_INDENT_; i++)
	{
		err_printf = printf(" ");
		ASSERT_PRINTF(err_printf);
	}
	err_printf = printf("\n");
	ASSERT_PRINTF(err_printf);
	tree_akin_print(print_this->yes_left, deep + 1, phrases);
	tree_akin_print(print_this->no_right, deep + 1, phrases);
	return 1;
}

tree_akin * read_tree(FILE * input)
{
	assert(input != NULL);
	char curr = 0;
	int number = 0;
	int err = 0;
	err = fscanf(input, "%c", &curr);
	assert(err > 0);
	if (curr == ')') return NULL;
	assert(curr == '(');
	tree_akin * leaf = calloc(1, sizeof(tree_akin));
	assert(leaf != NULL);
	err = fscanf(input, "%d", &number);
	assert(err >= 0);
	leaf->phrase = number;
	leaf->yes_left = read_tree(input);
	if (leaf->yes_left != NULL)
	{
		leaf->no_right = read_tree(input);
		assert(leaf->no_right != NULL);
		err = fscanf(input, "%c", &curr);
		assert(curr == ')');
	}
	return leaf;
}