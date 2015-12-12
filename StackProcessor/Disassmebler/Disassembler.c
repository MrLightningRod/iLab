#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "C:\\Resources\commands.h"


void Set_Com_List(_COM_NAMES_ **com_list);
int Com_Cmp(const void *first, const void *second);
char *Read_File();


int main()
{

	FILE *code = fopen("C:\\Resources\\code.txt", "wb");
	assert(code);
	unsigned int memory_used = 0;
	unsigned int current_command = 0;
	int current_data = 0;
	int current_data_num = 0;

	_COM_NAMES_ **init_com_list = calloc(_COM_NUMBER_, sizeof(_COM_NAMES_*));
	Set_Com_List(init_com_list);


	//************** WORKING WITH FILE **************//

	void *current = Read_File();

	//*************** FILE WORKING FINISHED ***************//


	//*************** ALERT! LARGE SWITCH WAS HERE! BE CAREFUL! ***********//

	do
	{
		current_command = *((char*)current);
		current = (char*)current + 1;
		fprintf(code, "%s", (init_com_list[current_command]->name));
		for (current_data_num = 0; current_data_num < (init_com_list[current_command]->arg_num); current_data_num++)
		{
			current_data = *((int*)current);
			current = (int*)current + 1;
			fprintf(code, "% d", current_data);
		}
		if (current_command != _COM_FINISH_) fprintf(code, "\n");
	} while (current_command != 0);

			//********** GRATZ! THIS IS OVER! *************//

	fclose(code);
	return 0;
}


char *Read_File()
{
	FILE *prog = fopen("C:\\Resources\\prog.nas", "rb");

	assert(prog);

	unsigned int size_of_file = 0;
	fseek(prog, 0, SEEK_END);
	size_of_file = ftell(prog);
	rewind(prog);
	char *command_list = calloc(size_of_file, sizeof(char));
	fread(command_list, sizeof(char), size_of_file, prog);
	void *current = command_list;
	fclose(prog);
	return command_list;
}

void Set_Com_List(_COM_NAMES_** com_list)
{
	assert(com_list);

	int i = 0;
	for (i = 0; i < _COM_NUMBER_; i++)
	{
		com_list[i] = &(_NAMES_OF_COM_[i]);
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