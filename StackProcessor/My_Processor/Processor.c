#include "My_Int_Stack.h"
#include "\Resources\commands.h"

#define _REGISTR_NUM_ 100
#define _COM_LIST_SIZE_ 128


typedef struct
{
	char *commands_beg;
	char *current;
	My_Int_Stack proc_stack;
	My_Int_Stack proc_ret;
	int registr[_REGISTR_NUM_];
} Processor;


Processor *Proc_New();
void Proc_Start(Processor *proc, char *commands);
char Proc_Next_Com(Processor *proc);
int Proc_Get_Data(Processor *proc);
void Proc_Ctor(Processor *construct_this);
void Proc_Dtor(Processor *destruct_this);
void Proc_Com_Push(Processor *proc);
int Proc_Com_Pop(Processor *proc);
int Proc_Com_Add(Processor *proc);
int Proc_Com_Sub(Processor *proc);
int Proc_Com_Mul(Processor *proc);
int Proc_Com_Div(Processor *proc);
int Proc_Com_Cmp(Processor *proc);
void Proc_Com_Jump(Processor *proc);
void Proc_Com_Je(Processor *proc);
void Proc_Com_Jne(Processor *proc);
void Proc_Com_Jg(Processor *proc);
void Proc_Com_Jge(Processor *proc);
void Proc_Com_Jl(Processor *proc);
void Proc_Com_Jle(Processor *proc);
void Proc_Com_Pushto(Processor *proc);
void Proc_Com_Call(Processor *proc);
void Proc_Com_Ret(Processor *proc);
int Proc_Com_Pushfrom(Processor *proc);
int Proc_Com_Out(Processor *proc);
int Proc_Com_In(Processor *proc);


char *Read_File()
{
	FILE *prog = fopen("\\Resources\\prog.nas", "rb");

	assert(prog);

	unsigned int size_of_file = 0;
	fseek(prog, 0, SEEK_END);
	size_of_file = ftell(prog);
	rewind(prog);
	char *command_list = calloc(size_of_file, sizeof(char));
	fread(command_list, sizeof(char), size_of_file, prog);
	fclose(prog);
	return command_list;
}


int main()
{

	Processor nas_edition_g1;
	Proc_Ctor(&nas_edition_g1);


	//************** WORKING WITH FILE **************//

	char *command_list = Read_File();

	//*************** FILE WORKING FINISHED ***************//


	Proc_Start(&nas_edition_g1, command_list);


	return 0;
}


void Proc_Start(Processor *proc, char *commands)
{
	proc->commands_beg = commands;
	proc->current = commands;
	char current_command = 0;

	//*************** ALERT! LARGE SWITCH! BE CAREFUL! ***********//

	do
	{
		current_command = Proc_Next_Com(proc);
		switch (current_command)
		{
		default:

			assert(!"ALERT! IT'S NOT PROGRAM! I'LL BE TERMINATED!");

			break;
		case _COM_PUSH_:
			Proc_Com_Push(proc);
			break;
		case _COM_POP_:
			Proc_Com_Pop(proc);
			break;
		case _COM_ADD_:
			Proc_Com_Add(proc);
			break;
		case _COM_SUB_:
			Proc_Com_Sub(proc);
			break;
		case _COM_MUL_:
			Proc_Com_Mul(proc);
			break;
		case _COM_DIV_:
			Proc_Com_Div(proc);
			break;
		case _COM_CMP_:
			Proc_Com_Cmp(proc);
			break;
		case _COM_JUMP_:
			Proc_Com_Jump(proc);
			break;
		case _COM_PUSHTO_:
			Proc_Com_Pushto(proc);
			break;
		case _COM_JE_:
			Proc_Com_Je(proc);
			break;
		case _COM_JNE_:
			Proc_Com_Jne(proc);
			break;
		case _COM_JG_:
			Proc_Com_Jg(proc);
			break;
		case _COM_JGE_:
			Proc_Com_Jge(proc);
			break;
		case _COM_JL_:
			Proc_Com_Jl(proc);
			break;
		case _COM_JLE_:
			Proc_Com_Jle(proc);
			break;
		case _COM_PUSHFROM_:
			Proc_Com_Pushfrom(proc);
			break;
		case _COM_OUT_:
			Proc_Com_Out(proc);
			break;
		case _COM_IN_:
			Proc_Com_In(proc);
			break;
		case _COM_CALL_:
			Proc_Com_Call(proc);
			break;
		case _COM_RET_:
			Proc_Com_Ret(proc);
		case _COM_FINISH_:
			break;
		}
	} while (current_command != 0);

	//********** GRATZ! THIS IS OVER! *************//
}


char Proc_Next_Com(Processor *proc)
{
	char current_com = *(proc->current);
	(proc->current)++;
	return current_com;
}


Processor *Proc_New()
{
	return (calloc(1, sizeof(Processor)));
}


void Proc_Ctor(Processor *construct_this)
{

	assert(construct_this);

	My_Int_Stack_Ctor(&(construct_this->proc_stack));
	My_Int_Stack_Ctor(&(construct_this->proc_ret));
	construct_this->commands_beg = 0;
	construct_this->current = 0;
}


void Proc_Dtor(Processor *destruct_this)
{

	assert(destruct_this);

	My_Int_Stack_Dtor(&(destruct_this->proc_stack));
	My_Int_Stack_Dtor(&(destruct_this->proc_ret));
}


int Proc_Get_Data(Processor *proc)
{

	assert(proc);

	int data = *((int*)proc->current);
	proc->current += sizeof(int);
	return data;
}


void Proc_Com_Push(Processor *proc)
{

	assert(proc);

	int push_this = Proc_Get_Data(proc);
	assert(proc);
	My_Int_Stack_Push(&(proc->proc_stack), push_this);
}


int Proc_Com_Pop(Processor *proc)
{

	assert(proc);

	int address = Proc_Get_Data(proc);
	return (proc->registr[address] = My_Int_Stack_Pop(&(proc->proc_stack)));
}


int Proc_Com_Add(Processor *proc)
{

	assert(proc);

	int first, second;
	first = My_Int_Stack_Pop(&(proc->proc_stack));
	second = My_Int_Stack_Pop(&(proc->proc_stack));
	return My_Int_Stack_Push(&(proc->proc_stack), first + second);
}


int Proc_Com_Sub(Processor *proc)
{

	assert(proc);

	int first, second;
	first = My_Int_Stack_Pop(&(proc->proc_stack));
	second = My_Int_Stack_Pop(&(proc->proc_stack));
	return My_Int_Stack_Push(&(proc->proc_stack), second - first);
}


int Proc_Com_Mul(Processor *proc)
{

	assert(proc);

	int first, second;
	first = My_Int_Stack_Pop(&(proc->proc_stack));
	second = My_Int_Stack_Pop(&(proc->proc_stack));
	return My_Int_Stack_Push(&(proc->proc_stack), first * second);
}


int Proc_Com_Div(Processor *proc)
{

	assert(proc);

	int first, second;
	first = My_Int_Stack_Pop(&(proc->proc_stack));
	second = My_Int_Stack_Pop(&(proc->proc_stack));
	return My_Int_Stack_Push(&(proc->proc_stack), second / first);
}


void Proc_Com_Pushto(Processor *proc)
{

	assert(proc);

	int address = Proc_Get_Data(proc);
	int push_this = Proc_Get_Data(proc);
	proc->registr[address] = push_this;
}


int Proc_Com_Cmp(Processor *proc)
{

	assert(proc);

	int second = My_Int_Stack_Pop(&(proc->proc_stack));
	int first = My_Int_Stack_Pop(&(proc->proc_stack));
	int result = 0;
	if (first < second) result = 1;
	if (first > second) result = -1;
	My_Int_Stack_Push(&(proc->proc_stack), result);
	return result;
}


void Proc_Com_Jump(Processor *proc)
{

	assert(proc);

	proc->current += sizeof(int);
	int jump = Proc_Get_Data(proc);
	proc->current = proc->commands_beg + jump;
}


void Proc_Com_Je(Processor *proc)
{

	assert(proc);

	int cmp = My_Int_Stack_Pop(&(proc->proc_stack));
	if (cmp == 0)
	{
		Proc_Com_Jump(proc);
	}
	else
	{
		proc->current += sizeof(int) * 2;
	}
}


void Proc_Com_Jne(Processor *proc)
{

	assert(proc);

	int cmp = My_Int_Stack_Pop(&(proc->proc_stack));
	if (cmp)
	{
		Proc_Com_Jump(proc);
	}
	else
	{
		proc->current += sizeof(int) * 2;
	}
}


void Proc_Com_Jg(Processor *proc)
{

	assert(proc);

	int cmp = My_Int_Stack_Pop(&(proc->proc_stack));
	if (cmp > 0)
	{
		Proc_Com_Jump(proc);
	}
	else
	{
		proc->current += sizeof(int) * 2;
	}
}


void Proc_Com_Jge(Processor *proc)
{

	assert(proc);

	int cmp = My_Int_Stack_Pop(&(proc->proc_stack));
	if (cmp >= 0)
	{
		Proc_Com_Jump(proc);
	}
	else
	{
		proc->current += sizeof(int) * 2;
	}
}


void Proc_Com_Jl(Processor *proc)
{

	assert(proc);

	int cmp = My_Int_Stack_Pop(&(proc->proc_stack));
	if (cmp < 0)
	{
		Proc_Com_Jump(proc);
	}
	else
	{
		proc->current += sizeof(int) * 2;
	}
}


void Proc_Com_Jle(Processor *proc)
{

	assert(proc);

	int cmp = My_Int_Stack_Pop(&(proc->proc_stack));
	if (cmp <= 0)
	{
		Proc_Com_Jump(proc);
	}
	else
	{
		proc->current += sizeof(int) * 2;
	}
}


int Proc_Com_Pushfrom(Processor *proc)
{

	assert(proc);

	int address = Proc_Get_Data(proc);
	return My_Int_Stack_Push(&(proc->proc_stack), proc->registr[address]);
}

int Proc_Com_Out(Processor *proc)
{
	assert(proc);

	int out = My_Int_Stack_Pop(&(proc->proc_stack));
	printf("%d\n", out);
	return out;
}

int Proc_Com_In(Processor *proc)
{
	assert(proc);

	int inp = 0;
	scanf("%d", &inp);
	My_Int_Stack_Push(&(proc->proc_stack), inp);
	return inp;
}

void Proc_Com_Call(Processor *proc)
{
	assert(proc);

	Proc_Get_Data(proc);
	My_Int_Stack_Push(&(proc->proc_ret), (proc->current - proc->commands_beg + sizeof(int)));
	proc->current = proc->commands_beg + Proc_Get_Data(proc);
}

void Proc_Com_Ret(Processor *proc)
{
	assert(proc);

	proc->current = proc->commands_beg + My_Int_Stack_Pop(&(proc->proc_ret));
}