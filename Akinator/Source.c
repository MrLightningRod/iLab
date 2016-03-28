#include "TreeAkin.h"
#include <locale.h>
#include <string.h>
#include <Windows.h>
#define ASSERT_BACKUP(x) assert((x != 0) || (!"Can't make backup file, it's not safety to continue"))

const int _BUF_SIZE_ = 1024;
const char * _TREE_FILE_ = "Tree.txt";
const char * _TREE_BACKUP_FILE_ = "Tree_Backup.txt";
const char * _PHRASES_FILE_ = "Phrases.txt";
const char * _PHRASES_BACKUP_FILE_ = "Phrases_Backup.txt";
const char _DELIMITER_ = '/';
const int _STR_SIZE_ = 40;

char * read_phrases(const char * file_name, int * phrases_size);
char ** parse_phrases(char * phrases, int phrases_size, int * number_of_phrases);
int backup(const char * name, const char * backup_name);
int play_akin(tree_akin * tree, char *** phrases_addr, int * number_of_phrases);
char ** resize_addr(char ** phrases_addr, int * size_of_addr);
tree_akin * akin_guess(tree_akin * tree, char ** phrases_addr);
int save_tree(const char * tree_file, const char * phrases_file, tree_akin * tree, char ** phrases, int phrases_numb);
int print_leaves(FILE * output, tree_akin * tree);

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	int err = 0;
	int backup_is_ok = 0;
	backup_is_ok = backup(_TREE_FILE_, _TREE_BACKUP_FILE_);
	ASSERT_BACKUP(backup_is_ok);
	backup_is_ok = backup(_PHRASES_FILE_, _PHRASES_BACKUP_FILE_);
	ASSERT_BACKUP(backup_is_ok);
	FILE * tree_f = fopen(_TREE_FILE_, "rb");
	assert(tree_f != NULL);
	tree_akin * tree = read_tree(tree_f);
	err = fclose(tree_f);
	assert(err == 0);
	int phrases_size = 0;
	int number_of_phrases = 0;
	char * phrases = read_phrases(_PHRASES_FILE_, &phrases_size);
	char ** phrases_addr = parse_phrases(phrases, phrases_size, &number_of_phrases);
	int number_of_init_phrases = number_of_phrases;
	while (play_akin(tree, &phrases_addr, &number_of_phrases));
	save_tree(_TREE_FILE_, _PHRASES_FILE_, tree, phrases_addr, number_of_phrases);


	tree_akin_dtor(tree);
	free(phrases);
	phrases = NULL;
	int i = 0;
	for (i = number_of_init_phrases; i < number_of_phrases; i++)
	{
		free(phrases_addr[i]);
	}
	free(phrases_addr);
	phrases_addr = NULL;
	return 0;
}

int backup(const char * name, const char * backup_name)
{
	assert(name);
	assert(backup_name);
	FILE * input = fopen(name, "rb");
	if (input == NULL) return 0;
	FILE * backup = fopen(backup_name, "wb");
	if (backup == NULL) return 0;
	char * bufer = calloc(_BUF_SIZE_, sizeof(char));
	if (bufer == NULL) return 0;
	int read_bytes = 0;
	int wrote_bytes = 0;
	while ((read_bytes = fread((void *)bufer, sizeof(char), _BUF_SIZE_, input)) > 0)
	{
		wrote_bytes = fwrite((const void *)bufer, sizeof(char), read_bytes, backup);
		if (wrote_bytes != read_bytes)
		{
			fclose(input);
			fclose(backup);
			free(bufer);
			return 0;
		}
	}
	if (read_bytes < 0)
	{
		fclose(input);
		fclose(backup);
		free(bufer);
		return 0;
	}
	int err = fclose(input);
	assert(err == 0);
	err = fclose(backup);
	assert(err == 0);
	free(bufer);
	return 1;
}

char * read_phrases(const char * file_name, int * phrases_size)
{
	FILE * input = fopen(file_name, "rb");
	assert(input != NULL);
	int err = 0;
	err = fseek(input, 0, SEEK_END);
	assert(err == 0);
	int size_of_file = ftell(input);
	assert(size_of_file != -1);
	*phrases_size = size_of_file / sizeof(char) + 1; // + 1 because of '/0' in the end
	rewind(input);
	assert(input != NULL);
	char * phrases = calloc(*phrases_size, sizeof(char));
	assert(phrases != NULL);
	int read_chars = fread(phrases, sizeof(char), *phrases_size - 1, input);
	assert(read_chars == *phrases_size - 1);
	err = fclose(input);
	assert(err == 0);
	return phrases;
}

char ** parse_phrases(char * phrases, int phrases_size, int * number_of_phrases)
{
	assert(phrases != NULL);
	assert(phrases_size > 0);
	*number_of_phrases = 1;
	int i = 0;
	for (i = 0; i < phrases_size; i++)
	{
		if (phrases[i] == _DELIMITER_)
		{
			(*number_of_phrases)++;
		}
	}
	char ** phrases_address = calloc(*number_of_phrases, sizeof(char **));
	assert(phrases_address != NULL);
	phrases_address[0] = strtok(phrases, &_DELIMITER_);
	assert(phrases_address[0] != NULL);
	for (i = 1; i < *number_of_phrases; i++)
	{
		phrases_address[i] = strtok(NULL, &_DELIMITER_);
		assert(phrases_address[i] != NULL);
	}
	return phrases_address;
}

int play_akin(tree_akin * tree, char *** phrases_addr, int * number_of_phrases)
{
	int err = 0;
	err = printf("Если хотите сыграть, введите \"Да\"\n");
	assert(err > 0);
	char inp[4];
	err = scanf("%3s", inp);
	assert(err >= 0);
	if (strcmp(inp, "Да") != 0) return 0;
	int size_of_addr = (*number_of_phrases) * sizeof(char **);
	err = printf("Загадайте какой-либо объект, введите любую букву и нажмите Enter, когда будете готовы\n");
	assert(err > 0);
	err = scanf("%1s", inp);
	assert(err >= 0);
	tree_akin * res = akin_guess(tree, *phrases_addr);
	if (res == NULL)
	{
		printf("Я угадал\n");
	}
	else
	{
		printf("Чем этот объект отличается от %s? (не более 40 символов)\n", ((*phrases_addr)[res->phrase]));
		if (*number_of_phrases == size_of_addr / sizeof(char **))
		{
			*phrases_addr = resize_addr(*phrases_addr, &size_of_addr);
		}
		(*phrases_addr)[*number_of_phrases] = (char *)calloc(_STR_SIZE_ + 1, sizeof(char));
		*number_of_phrases = *number_of_phrases + 1;
		scanf("%40s", (*phrases_addr)[*number_of_phrases - 1]);
		res->no_right = calloc(1, sizeof(tree_akin));
		res->no_right->no_right = NULL;
		res->no_right->yes_left = NULL;
		res->no_right->phrase = res->phrase;
		res->phrase = *number_of_phrases - 1;
		res->yes_left = calloc(1, sizeof(tree_akin));
		res->yes_left->no_right = NULL;
		res->yes_left->yes_left = NULL;
		(*phrases_addr)[*number_of_phrases] = (char *)calloc(_STR_SIZE_ + 1, sizeof(char));
		*number_of_phrases = *number_of_phrases + 1;
		res->yes_left->phrase = *number_of_phrases - 1;
		printf("Что это такое? (не более 40 символов)\n");
		scanf("%40s", (*phrases_addr)[*number_of_phrases - 1]);
		printf("Я запомню\n");
	}


	return 1;
}

char ** resize_addr(char ** phrases_addr, int * size_of_addr)
{
	(*size_of_addr) = (*size_of_addr) * 2;
	return (realloc((void *)phrases_addr, *size_of_addr));
}

tree_akin * akin_guess(tree_akin * tree, char ** phrases_addr)
{
	assert(tree != NULL);
	assert(phrases_addr != NULL);
	int yes_no = 0;
	int err = printf("Это %s? (Да/Нет)\n", phrases_addr[tree->phrase]);
	assert(err > 0);
	char inp[4];
	err = scanf("%3s", inp);
	assert(err >= 0);
	if (strcmp(inp, "Да") == 0) yes_no = 1;
	tree_akin * res = NULL;
	if ((tree->yes_left != NULL) && (yes_no)) res = akin_guess(tree->yes_left, phrases_addr);
	if ((tree->no_right != NULL) && (!yes_no)) res = akin_guess(tree->no_right, phrases_addr);
	if ((tree->no_right == NULL) && (!yes_no)) res = tree;
	return res;
}

int save_tree(const char * tree_file, const char * phrases_file, tree_akin * tree, char ** phrases, int phrases_numb)
{
	FILE * tree_f = fopen(tree_file, "wb");
	FILE * phrases_f = fopen(phrases_file, "wb");
	print_leaves(tree_f, tree);
	int i = 0;
	for (i = 0; i < phrases_numb; i++)
	{
		if (i != 0)
		{
			fprintf(phrases_f, "/");
		}
		fprintf(phrases_f, "%s", phrases[i]);
	}

	fclose(tree_f);
	fclose(phrases_f);
	return 1;
}

int print_leaves(FILE * output, tree_akin * tree)
{
	if (tree == NULL) return 1;
	fprintf(output, "(%d", tree->phrase);
	print_leaves(output, tree->yes_left);
	print_leaves(output, tree->no_right);
	fprintf(output, ")");
	return 1;
}