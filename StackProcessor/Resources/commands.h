#pragma once

const unsigned int _COM_NUMBER_ = 22;

enum
{
	_COM_FINISH_   = 0 ,
	_COM_PUSH_     = 1 ,
	_COM_POP_      = 2 ,
	_COM_ADD_      = 3 ,
	_COM_SUB_      = 4 ,
	_COM_MUL_      = 5 ,
	_COM_DIV_      = 6 ,
	_COM_JUMP_     = 7 ,
	_COM_LABEL_    = 8 ,
	_COM_PUSHTO_   = 9 ,
	_COM_CMP_      = 10,
	_COM_JE_       = 11,
	_COM_JNE_      = 12,
	_COM_JG_       = 13,
	_COM_JGE_      = 14,
	_COM_JL_       = 15,
	_COM_JLE_      = 16,
	_COM_PUSHFROM_ = 17,
	_COM_OUT_      = 18,
	_COM_CALL_     = 19,
	_COM_RET_      = 20,
	_COM_IN_       = 21
};

typedef struct
{
	int num;
	int arg_num;
	const char* name;
	int is_jump;
} _COM_NAMES_;

const _COM_NAMES_ _NAMES_OF_COM_ [] =
{
	{ _COM_FINISH_,   0, "finish",   0 },
	{ _COM_PUSH_,     1, "push",     0 },
	{ _COM_POP_,      1, "pop",      0 },
	{ _COM_ADD_,      0, "add",      0 },
	{ _COM_SUB_,      0, "sub",      0 },
	{ _COM_MUL_,      0, "mul",      0 },
	{ _COM_DIV_,      0, "div",      0 },
	{ _COM_JUMP_,     2, "jump",     1 },
	{ _COM_LABEL_,    1, "label",    0 },
	{ _COM_PUSHTO_,   2, "pushto",   0 },
	{ _COM_CMP_,      0, "cmp",      0 },
	{ _COM_JE_,       2, "je",       1 },
	{ _COM_JNE_,      2, "jne",      1 },
	{ _COM_JG_,       2, "jg",       1 },
	{ _COM_JGE_,      2, "jge",      1 },
	{ _COM_JL_,       2, "jl",       1 },
	{ _COM_JLE_,      2, "jle",      1 },
	{ _COM_PUSHFROM_, 1, "pushfrom", 0 },
	{ _COM_OUT_,      0, "out",      0 },
	{ _COM_CALL_,     2, "call",     1 },
	{ _COM_RET_,      0, "return",   0 },
	{ _COM_IN_,       0, "in",       0 }
};