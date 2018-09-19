#include "stdafx.h"

#include <windows.h>

#include "Misc.h"
#include "XBoard.h"

int _tmain(int argc, _TCHAR* argv[])
{
	XInputEnable(true);

	int lines = 0;
	bool spamming = false;
	DWORD GetStateResult = 0;
	DWORD PadNumber = 0;

	XINPUT_STATE state;
	ZeroMemory(&state, sizeof(XINPUT_STATE));
	XINPUT_STATE laststate;
	ZeroMemory(&laststate, sizeof(XINPUT_STATE));

	struct ExecThreadArgs ThreadDataArray[MAX_BINDS];
	ZeroMemory(ThreadDataArray, sizeof(ThreadDataArray));

	INPUT InptArray[MAX_BINDS];
	ZeroMemory(InptArray, sizeof(InptArray));

	int BindCount = 0;

	lines = ParseConfigFile(ThreadDataArray);

	for (int i = 0; i < MAX_BINDS;)
	{
		if (ThreadDataArray[i].bind != NULL)
		{
			ThreadDataArray[i].state = &state;
			ThreadDataArray[i].InptArrayEntry = &InptArray[BindCount];
			SpawnExecThread(ThreadDataArray[i]);
			BindCount++;
		}
		i++;
	}

	int running = 1;
	while (running)
	{
		GetStateResult = XInputGetState(PadNumber, &state);

		if (GetStateResult == ERROR_SUCCESS)
		{
			spamming = false;

			if (state.dwPacketNumber != laststate.dwPacketNumber)
			{
				laststate = state;
			}
		}
		else // Get State Failed
		{
			if (spamming == false)
			{
				printf("Failed to get control state \n");
				spamming = true;
			}
		}
	}
	for (int i = 0; i < MAX_BINDS;)
	{
		delete ThreadDataArray[i].bind;
		i++;
	}
	return 0;
}