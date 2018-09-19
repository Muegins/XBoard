#ifndef EXECTHREADARGS_H
#define EXECTHREADARGS_H

#include "windows.h"
#include "Xinput.h"
#include "ButtonBind.h"


#define MAX_BINDS 28

struct ExecThreadArgs
{
	ButtonBind* bind;
	XINPUT_STATE* state;
	INPUT* InptArrayEntry;
};

#endif //!EXECTHREADARGS_H