#ifndef __MACRO_H__
#define __MACRO_H__

#define SWAP(Type, VariableOne, VariableTwo)	\
{						\
	Type Temp = VariableOne;		\
	VariableOne = VariableTwo;		\
	VariableTwo = Temp;			\
}

#endif
