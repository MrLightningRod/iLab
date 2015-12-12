#pragma once
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>


#ifndef SUPERDEBUG
#define SUPERDEBUG 1
#endif


#ifdef SUPERDEBUG
#ifndef MY_STACK_DEBUG
#define MY_STACK_DEBUG(x) assert(My_Int_Stack_Is_Ok(x));
#endif
#else
#ifndef MY_STACK_DEBUG
#define MY_DEBUG(x) ;
#endif
#endif


typedef struct
{
	size_t number_of_elements;
	size_t current_size;
	int *stack_beg;
} My_Int_Stack;


int My_Int_Stack_Is_Ok(My_Int_Stack *check_this);
My_Int_Stack *New_My_Int_Stack();
My_Int_Stack *My_Int_Stack_Ctor(My_Int_Stack *construct_this);
void *My_Int_Stack_Dtor(My_Int_Stack *destruct_this);
int My_Int_Stack_Push(My_Int_Stack *this_stack, int add_this);
int My_Int_Stack_Is_Empty(My_Int_Stack *check_this);
int My_Int_Stack_Top(My_Int_Stack *take_this);
int My_Int_Stack_Pop(My_Int_Stack *pop_this);