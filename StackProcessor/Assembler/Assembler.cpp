#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <map>
#include "\Resources\commands.h"

using namespace std;


const int _INITIAL_LIST_SIZE_ = 512;
const int _HASH_MOD_ = 1000000007;


int my_hash(char* hash_this);


typedef struct
{
	unsigned int current_size;
	unsigned int memory_used;
	char *command_list;
} list_of_commands;

char *find_command(unsigned int command, _COM_NAMES_** com_list);
void hash_commands(int *hashes, _COM_NAMES_** com_list);
list_of_commands* List_Com_New_Ctor();
void List_Com_Push_Com(list_of_commands *this_list, char push_this);
void List_Com_Push_Data(list_of_commands *this_list, int push_this);
void List_Com_Jumpes(list_of_commands *this_list, map <int, int> *labels, _COM_NAMES_ **com_list);
void Set_Com_List(_COM_NAMES_ **com_list);
int Com_Cmp(const void *first, const void *second);
void Set_Com_Map(map <int, int> *table, int *hashes);

int main()
{
	FILE *code = fopen("\\Resources\\code.txt", "rb");
	FILE *assembler = fopen("\\Resources\\prog.nas", "wb");

	assert(code);
	assert(assembler);

	_COM_NAMES_ **init_com_list = (_COM_NAMES_**)calloc(_COM_NUMBER_, sizeof(_COM_NAMES_*));
	Set_Com_List(init_com_list);

	int *hashes = (int*)calloc(_COM_NUMBER_ + 1, sizeof(int));
	char command[100];


	//***********WRITING HASHES***********//

	hash_commands(hashes, init_com_list);

	//************************************//

	map <int, int> table_com;
	Set_Com_Map(&table_com, hashes);

	map <int, int> table_labels;

	int current_hash = 0;
	int current_com = 0;
	int current_data = 0;
	int number_of_data = 0;
	int next_label = 0;
	char *print_this = NULL;
	list_of_commands *commands = List_Com_New_Ctor();
	assert(commands);


	//***********************COMPILATION***********************//


	while (!feof(code))
	{
		fgets(command, 100, code);
		current_hash = my_hash(strtok(command, " \r\n"));
		current_com = table_com[current_hash];
		switch (current_com)
		{
		default:
			List_Com_Push_Com(commands, (char)current_com);
			for (number_of_data = 0; number_of_data < init_com_list[current_com]->arg_num; number_of_data++)
			{
				if ((init_com_list[current_com]->is_jump) && (number_of_data == 1))
				{
					current_data = my_hash(strtok(NULL, " \r\n"));
				}
				else
				{
					current_data = strtol(strtok(NULL, " \r\n"), &print_this, 10);
				}
				List_Com_Push_Data(commands, current_data);
			}
			break;
		case _COM_LABEL_:
			int label_hash = my_hash(strtok(NULL, " \r\n"));
			table_labels.insert(pair <int, int>(label_hash, commands->memory_used));
			break;
		}
	}
	List_Com_Jumpes(commands, &table_labels, init_com_list);


	//**************************COMPILATION FINISHED***************************//


	fwrite(commands->command_list, commands->current_size, 1, assembler);


	fclose(code);
	fclose(assembler);
	free(init_com_list);
	free(hashes);
	return 0;
}


int my_hash(char* hash_this)
{
	assert(hash_this);
	int hash = 0;
	int i = 0;
	while (hash_this[i])
	{
		hash += hash_this[i];
		hash += (hash << 10);
		hash ^= (hash >> 6);
		i++;
	}
	hash += (hash << 3);
	hash ^= (hash >> 11);
	hash += (hash << 15);
	return hash;
}

char *find_command(unsigned int command, _COM_NAMES_** com_list)
{
	assert((command < _COM_NUMBER_) && (command >= 0));

	char *this_name = (char*)com_list[command]->name;
	return this_name;
}

void hash_commands(int *hashes, _COM_NAMES_** com_list)
{
	assert(hashes);

	int i = 0;
	char *name = NULL;
	for (i = 0; i < _COM_NUMBER_; i++)
	{
		name = find_command(i, com_list);
		assert(name);
		hashes[i] = my_hash(name);
	}
}

list_of_commands* List_Com_New_Ctor()
{
	list_of_commands* new_list = (list_of_commands*)calloc(1, sizeof(list_of_commands));
	new_list->command_list = (char*)calloc(_INITIAL_LIST_SIZE_, sizeof(char));
	new_list->memory_used = 0;
	new_list->current_size = _INITIAL_LIST_SIZE_;
	return new_list;
}

void List_Com_Push_Com(list_of_commands *this_list, char push_this)
{
	assert(this_list);

	this_list->command_list[this_list->memory_used] = push_this;
	this_list->memory_used += sizeof(char);
	if (this_list->memory_used >= this_list->current_size / 2)
	{
		this_list->current_size *= 2;
		this_list->command_list = (char*)realloc(this_list->command_list, this_list->current_size);
	}

	assert(this_list->memory_used <= this_list->current_size);
}

void List_Com_Push_Data(list_of_commands *this_list, int push_this)
{
	assert(this_list);

	*(int*)(this_list->command_list + this_list->memory_used) = push_this;
	this_list->memory_used += sizeof(int);
	if (this_list->memory_used >= this_list->current_size / 2)
	{
		this_list->current_size *= 2;
		this_list->command_list = (char*)realloc(this_list->command_list, this_list->current_size);
	}

	assert(this_list->memory_used <= this_list->current_size);

}

void List_Com_Jumpes(list_of_commands *this_list, map<int, int> *labels, _COM_NAMES_** com_list)
{
	assert(this_list);
	assert(labels);

	char *current = this_list->command_list;

	assert(current);

	int current_data = 0;
	int *current_data_ptr = NULL;
	while (*current != _COM_FINISH_)
	{
		if (com_list[*current]->is_jump)
		{
			current_data_ptr = (int*)((char*)current + 1);
			current_data = *current_data_ptr;
			if (current_data)
			{
				*current_data_ptr = 0;
				current_data_ptr++;
				current_data = *current_data_ptr;

				int is_in = (*labels).find(current_data) != (*labels).end();
				assert(is_in);

				*current_data_ptr = (*labels)[current_data];
			}
		}
		current += sizeof(char) + sizeof(int) * com_list[*current]->arg_num;
	}
}

void Set_Com_List(_COM_NAMES_** com_list)
{
	assert(com_list);

	int i = 0;
	for (i = 0; i < _COM_NUMBER_; i++)
	{
		com_list[i] = (_COM_NAMES_*)&(_NAMES_OF_COM_[i]);
	}
	qsort(com_list, _COM_NUMBER_, sizeof(_COM_NAMES_*), Com_Cmp);
}

int Com_Cmp(const void *first, const void *second)
{
	assert(first);
	assert(second);

	_COM_NAMES_ *fr = (_COM_NAMES_*)first;
	_COM_NAMES_ *sc = (_COM_NAMES_ *)second;
	return (fr->num - sc->num);
}

void Set_Com_Map(map <int, int> *table, int *hashes)
{
	assert(table);
	assert(hashes);

	for (int i = 0; i < _COM_NUMBER_; i++)
	{
		(*table).insert(pair <int, int>(hashes[i], i));
	}
}