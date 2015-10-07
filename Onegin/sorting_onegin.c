#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <assert.h>
#include <sys/stat.h>
#include <stdlib.h>

int My_Compare(const void* first, const void* second);
int My_Compare_Reverse(const void* first, const void* second);
int Is_Alpha(char ch);
char To_Upper(char ch);

int main()
{
	setlocale(LC_ALL, "en_US.utf-8");
	FILE *our_onegin = NULL;
	int file_is_opened = 0;
	int very_long_name = 1;
	int i = 0;
	char part_of_file_name = 'a';
	char file_name[101];
	struct stat stats_of_file;
	unsigned long long size_of_file = 0;
	while (!file_is_opened || very_long_name)
	{
		part_of_file_name = 'a';
		i = 0;
		very_long_name = 1;
		printf("%s", "Input the name of file with poem (<= 100 symbols)\n");
		while (part_of_file_name)
		{
			scanf("%c", &part_of_file_name);
			if (part_of_file_name == '\n') part_of_file_name = '\0';
			if (i <= 100) file_name[i] = part_of_file_name;
			i++;
		}
		if (i <= 101) very_long_name = 0;
		if (very_long_name)
		{
			printf("%s", "Very long name of file, try to rename it, please\n");
		}
		else
		{
			our_onegin = (fopen(file_name, "rb"));
			file_is_opened = (our_onegin != NULL);
			if (!our_onegin) printf("%s", "I can't find the file in my directory\n");
		}
	}
	assert(!very_long_name);
	assert(file_is_opened);
	assert(our_onegin);
	stat(file_name, &stats_of_file);
	size_of_file = stats_of_file.st_size;
	char *onegin = (char*)calloc(size_of_file + 1, sizeof(char));
	assert(onegin);
	fread((void*)onegin, (size_t)size_of_file, 1, our_onegin);
	fclose(our_onegin);
	onegin[size_of_file] = '\0';
	i = 0;
	unsigned long long number_of_strings = 1;
	while (onegin[i])
	{
		if (onegin[i] == '\n')
		{
			number_of_strings++;
		}
		i++;
	}
	char **text = calloc(number_of_strings, sizeof(char*));
	char **text_sorted = calloc(number_of_strings, sizeof(char*));
	char **text_reverse_sorted = calloc(number_of_strings, sizeof(char*));
	assert(text);
	assert(text_sorted);
	assert(text_reverse_sorted);
	char *is_lexeme = strtok(onegin, "\n");
	assert(is_lexeme);
	i = 0;
	while (is_lexeme)
	{
		text[i] = is_lexeme;
		i++;
		is_lexeme = strtok(NULL, "\n");
	};
	assert(text);
	for (i = 0; i < number_of_strings; i++) text_sorted[i] = text[i];
	for (i = 0; i < number_of_strings; i++) text_reverse_sorted[i] = text[i];
	qsort(text_sorted, number_of_strings, sizeof(char**), My_Compare);
	qsort(text_reverse_sorted, number_of_strings, sizeof(char**), My_Compare_Reverse);
	our_onegin = fopen("output.txt", "wb");
	assert(our_onegin);
	for (i = 0; i < number_of_strings; i++) fprintf(our_onegin, "%s\n", text_sorted[i]);
	fclose(our_onegin);
	our_onegin = fopen("output_reverse.txt", "wb");
	assert(our_onegin);
	for (i = 0; i < number_of_strings; i++) fprintf(our_onegin, "%s\n", text_reverse_sorted[i]);
	fclose(our_onegin);
	return 0;
}

int Is_Alpha(char ch)
{
	return ((('a' <= ch) && (ch <= 'z')) || (('à' <= ch) && (ch <= 'ÿ')) || (('A' <= ch) && (ch <= 'Z')) || (('À' <= ch) && (ch <= 'ß')));
}

char To_Upper(char ch)
{
	if (('a' <= ch) && (ch <= 'z')) ch += 'A' - 'a';
	if (('à' <= ch) && (ch <= 'ÿ')) ch += 'À' - 'à';
	return ch;
}

int My_Compare(const void* arg1, const void* arg2)
{
	assert(arg1);
	assert(arg2);
	char *first = *(char**)arg1;
	char *second = *(char**)arg2;
	assert(first);
	assert(second);
	int first_len = strlen(first);
	int second_len = strlen(second);
	int max_length = ((first_len > second_len) ? first_len : second_len);
	char *first_my = (char*)calloc(max_length + 1, sizeof(char));
	char *second_my = (char*)calloc(max_length + 1, sizeof(char));
	assert(first_my);
	assert(second_my);
	int i = 0;
	int was_alpha = 0;
	while (*first)
	{
		if ((Is_Alpha(*first)) || ((*first == ' ') && (was_alpha)))
		{
			was_alpha = 1;
			*(first_my + i) = To_Upper(*first);
			i++;
		}
		first++;
	}
	*(first_my + i) = '\0';
	i = 0;
	was_alpha = 0;
	while (*second)
	{
		if ((Is_Alpha(*second)) || ((*second == ' ') && (was_alpha)))
		{
			was_alpha = 1;
			*(second_my + i) = To_Upper(*second);
			i++;
		}
		second++;
	}
	*(second_my + i) = '\0';
	return (strcmp(first_my, second_my));
}

int My_Compare_Reverse(const void* arg1, const void* arg2)
{
	assert(arg1);
	assert(arg2);
	char *first = *(char**)arg1;
	char *second = *(char**)arg2;
	assert(first);
	assert(second);
	int first_len = strlen(first);
	int second_len = strlen(second);
	int max_length = ((first_len > second_len) ? first_len : second_len);
	char *first_my = (char*)calloc(max_length + 1, sizeof(char));
	char *second_my = (char*)calloc(max_length + 1, sizeof(char));
	assert(first_my);
	assert(second_my);
	int i = 0;
	int was_alpha = 0;
	while (first_len + 1)
	{
		if (Is_Alpha(*(first + first_len)) || ((*(first + first_len) == ' ') && (was_alpha)))
		{
			was_alpha = 1;
			*(first_my + i) = To_Upper(*(first + first_len));
			i++;
		}
		first_len--;
	}
	*(first_my + i) = '\0';
	i = 0;
	was_alpha = 0;
	while (second_len + 1)
	{
		if (Is_Alpha(*(second + second_len)) || ((*(second + second_len) == ' ') && (was_alpha)))
		{
			was_alpha = 1;
			*(second_my + i) = To_Upper(*(second + second_len));
			i++;
		}
		second_len--;
	}
	*(second_my + i) = '\0';
	return (strcmp(first_my, second_my));
}