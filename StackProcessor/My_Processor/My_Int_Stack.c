#include "My_Int_Stack.h"

const size_t _INITIAL_SIZE_ = 128;
const int _INVALID_FOR_STACK_ = -1;

int My_Int_Stack_Is_Ok(My_Int_Stack *check_this)
{
	return ((check_this) &&
		(check_this->stack_beg) &&
		(check_this->current_size > 0) &&
		(check_this->number_of_elements >= 0) &&
		(check_this->current_size >= check_this->number_of_elements));
}

My_Int_Stack *New_My_Int_Stack()
{
	My_Int_Stack *new_stack = malloc(sizeof(My_Int_Stack));
	new_stack->current_size = _INVALID_FOR_STACK_;
	new_stack->number_of_elements = _INVALID_FOR_STACK_;
	return new_stack;
}

My_Int_Stack *My_Int_Stack_Ctor(My_Int_Stack *construct_this)
{
	assert(construct_this);
	construct_this->current_size = _INITIAL_SIZE_;
	construct_this->stack_beg = calloc(_INITIAL_SIZE_, sizeof(int));
	construct_this->number_of_elements = 0;
	MY_STACK_DEBUG(construct_this);
	return construct_this;
}

void *My_Int_Stack_Dtor(My_Int_Stack *destruct_this)
{
	assert(destruct_this);
	MY_STACK_DEBUG(destruct_this);
	free(destruct_this->stack_beg);
	destruct_this->current_size = _INVALID_FOR_STACK_;
	destruct_this->number_of_elements = _INVALID_FOR_STACK_;
	free(destruct_this);
	MY_STACK_DEBUG(destruct_this);
	return destruct_this;
}

int My_Int_Stack_Push(My_Int_Stack *this_stack, int add_this)
{
	assert(this_stack);
	MY_STACK_DEBUG(this_stack);
	if (this_stack->number_of_elements == this_stack->current_size)
	{
		this_stack->current_size *= 2;
		this_stack->stack_beg = realloc(this_stack->stack_beg, this_stack->current_size * sizeof(int));
	}
	this_stack->stack_beg[this_stack->number_of_elements] = add_this;
	this_stack->number_of_elements++;
	MY_STACK_DEBUG(this_stack);
	return add_this;
}

int My_Int_Stack_Is_Empty(My_Int_Stack *check_this)
{
	assert(check_this);
	MY_STACK_DEBUG(check_this);
	return (check_this->number_of_elements == 0);
}

int My_Int_Stack_Top(My_Int_Stack *take_this)
{
	assert(take_this);
	MY_STACK_DEBUG(take_this);
	return take_this->stack_beg[take_this->number_of_elements - 1];
}

int My_Int_Stack_Pop(My_Int_Stack *pop_this)
{
	assert(pop_this);
	MY_STACK_DEBUG(pop_this);
	assert(!My_Int_Stack_Is_Empty(pop_this));
	pop_this->number_of_elements--;
	int top = pop_this->stack_beg[pop_this->number_of_elements];
	if ((pop_this->number_of_elements <= pop_this->current_size / 4) && (pop_this->current_size > _INITIAL_SIZE_))
	{
		pop_this->current_size /= 2;
		pop_this->stack_beg = realloc(pop_this->stack_beg, pop_this->current_size * sizeof(int));
	}
	MY_STACK_DEBUG(pop_this);
	return top;
}
