#include "stdafx.h"
#include <windows.h>

#include "Misc.h"
#include "XBThreads.h"

using namespace std;

mutex stateMTX;

#define CURSOR_MAX_SPEED 10 //Measured in pixels/10 millis
#define THUMBSTICK_MAX 32767 //Maximum value of thumbstick output
#define DEADZONE 10000 //Measured in same units as thumbstick
#define AUTOREPEAT_INTERVAL 20 //millis
#define TRIGGER_THRESHOLD 15 //min 0(unpressed) max 255(fully pressed)
#define THREAD_REST_TIME 10 //millis


DWORD WINAPI tExecStandard(void* vETA)
{
	ExecThreadArgs* eta = ((ExecThreadArgs*)vETA);

	assert(eta != NULL);
	assert(eta->bind != NULL);

	XINPUT_STATE laststate;
	ZeroMemory(&laststate, sizeof(XINPUT_STATE));

	INPUT input;
	ZeroMemory(&input, sizeof(INPUT));

	int inputssent = 0;

	printf("Starting Standard Thread \n");

	input.type = INPUT_KEYBOARD;
	input.ki.dwFlags = eta->bind->GetKeyFlags();
	input.ki.wVk = eta->bind->GetVK_Key();
	input.ki.wScan = eta->bind->GetSC_Key();

	DWORD AutorepeatTimer = 0;
	bool KeyDown = false;

	bool running = true;
	while (running)
	{
		stateMTX.lock();
		//		printf("current state wButtons = %i \n", eta->state->Gamepad.wButtons & eta->bind->GetButton());
		//		printf("last state wButtons    = %i \n", laststate.Gamepad.wButtons & eta->bind->GetButton());
		//		printf("\n");
		if ((laststate.Gamepad.wButtons & eta->bind->GetButton()) != (eta->state->Gamepad.wButtons & eta->bind->GetButton())) // if the bound button has changed state
		{
			laststate.Gamepad = eta->state->Gamepad;
			laststate.dwPacketNumber = eta->state->dwPacketNumber;

			if ((eta->bind->GetButton() & eta->state->Gamepad.wButtons) == 0) // if the bound button is up
			{
				input.ki.dwFlags |= KEYEVENTF_KEYUP;
				KeyDown = false;
			}
			else //the bound button is down
			{
				input.ki.dwFlags &= ~KEYEVENTF_KEYUP;
				KeyDown = true;
			}
			SendInput(1, &input, sizeof(input));
		}
		stateMTX.unlock();
		Sleep(THREAD_REST_TIME);

		if (KeyDown == true)
		{
			if ((GetTickCount() - AutorepeatTimer) >= AUTOREPEAT_INTERVAL)
			{
				SendInput(1, &input, sizeof(input));
				AutorepeatTimer = GetTickCount();
			}
		}
	}
	return 0;
}

DWORD WINAPI tExecToggle(void* vETA)
{
	ExecThreadArgs* eta = ((ExecThreadArgs*)vETA);

	assert(eta != NULL);
	assert(eta->bind != NULL);

	XINPUT_STATE laststate;
	ZeroMemory(&laststate, sizeof(XINPUT_STATE));

	INPUT input;
	ZeroMemory(&input, sizeof(INPUT));

	printf("Starting Toggle Thread \n");

	input.type = INPUT_KEYBOARD;
	input.ki.dwFlags = eta->bind->GetKeyFlags();
	input.ki.wVk = eta->bind->GetVK_Key();
	input.ki.wScan = eta->bind->GetSC_Key();

	DWORD AutorepeatTimer = 0;

	bool running = true;
	while (running)
	{
		stateMTX.lock();
		if ((laststate.Gamepad.wButtons & eta->bind->GetButton()) != (eta->state->Gamepad.wButtons & eta->bind->GetButton()))
		{
			laststate.Gamepad = eta->state->Gamepad;
			laststate.dwPacketNumber = eta->state->dwPacketNumber;

			if (eta->state->Gamepad.wButtons &= eta->bind->GetButton())
			{
				if (eta->bind->GetTogState() == true)
				{
					input.ki.dwFlags |= KEYEVENTF_KEYUP;
					eta->bind->SetTogState(false);
				}
				else
				{
					input.ki.dwFlags &= ~KEYEVENTF_KEYUP;
					eta->bind->SetTogState(true);
				}

				SendInput(1, &input, sizeof(input));
			}
		}

		if (eta->bind->GetTogState() == true)
		{
			if ((GetTickCount() - AutorepeatTimer) >= AUTOREPEAT_INTERVAL)
			{
				SendInput(1, &input, sizeof(input));
				AutorepeatTimer = GetTickCount();
			}
		}
		stateMTX.unlock();
		Sleep(THREAD_REST_TIME);
	}
	return 0;
}

DWORD WINAPI tExecST_Dir(void* vETA)
{
	ExecThreadArgs* eta = ((ExecThreadArgs*)vETA);

	assert(eta != NULL);
	assert(eta->bind != NULL);

	XINPUT_STATE laststate;
	ZeroMemory(&laststate, sizeof(XINPUT_STATE));

	INPUT input;
	ZeroMemory(&input, sizeof(INPUT));

	SHORT* stick = NULL;

	printf("Starting Stick Direction Thread \n");

	input.type = INPUT_KEYBOARD;
	input.ki.dwFlags = eta->bind->GetKeyFlags();
	input.ki.wVk = eta->bind->GetVK_Key();
	input.ki.wScan = eta->bind->GetSC_Key();

	stateMTX.lock();
	if (eta->bind->GetStick() == LEFT_ST)
	{
		if ((eta->bind->GetDir() == UP) || (eta->bind->GetDir() == DOWN))
		{
			stick = &(eta->state->Gamepad.sThumbLY);
		}
		else if ((eta->bind->GetDir() == LEFT) || (eta->bind->GetDir() == RIGHT))
		{
			stick = &(eta->state->Gamepad.sThumbLX);
		}
		else
		{
			printf("Direction Thread Failed, Invalid Direction \n");
		}
	}
	else if (eta->bind->GetStick() == RIGHT_ST)
	{
		if ((eta->bind->GetDir() == UP) || (eta->bind->GetDir() == DOWN))
		{
			stick = &(eta->state->Gamepad.sThumbRY);
		}
		else if ((eta->bind->GetDir() == LEFT) || (eta->bind->GetDir() == RIGHT))
		{
			stick = &(eta->state->Gamepad.sThumbRX);
		}
		else
		{
			printf("Direction Thread Failed, Invalid Direction \n");
		}
	}
	else
	{
		printf("Direction Thread Failed, Invalid Stick \n");
	}
	stateMTX.unlock();

	bool SendKeyUp = false;
	DWORD AutorepeatTimer = 0;

	bool running = true;
	while (running)
	{

		int stickval = *stick;
		//		printf("stickval w/o flip = %i \n", stickval);

		if ((eta->bind->GetDir() == DOWN) || (eta->bind->GetDir() == LEFT))
		{
			stickval = -stickval;
			//			printf("stickval w/ flip = %i \n", stickval);
		}

		if (stickval < DEADZONE)
		{
			input.ki.dwFlags |= KEYEVENTF_KEYUP;
			if (SendKeyUp == true)
			{
				SendKeyUp = false;
				SendInput(1, &input, sizeof(input));
				//			printf("Key Up \n");
			}
		}
		else
		{
			input.ki.dwFlags &= ~KEYEVENTF_KEYUP;
			SendKeyUp = true;
			if ((GetTickCount() - AutorepeatTimer) >= AUTOREPEAT_INTERVAL)
			{
				SendInput(1, &input, sizeof(input));
				AutorepeatTimer = GetTickCount();
			}
		}


	}
	return 0;
}

DWORD WINAPI tExecST_Mouse(void* vETA)
{
	ExecThreadArgs* eta = ((ExecThreadArgs*)vETA);

	assert(eta != NULL);
	assert(eta->bind != NULL);

	XINPUT_STATE laststate;
	ZeroMemory(&laststate, sizeof(XINPUT_STATE));

	INPUT input;
	ZeroMemory(&input, sizeof(INPUT));

	float ThumbX = 0;
	float ThumbY = 0;

	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_MOVE;

	printf("Starting Mouse Thread \n");

	bool running = true;
	while (running)
	{
		if (eta->state->dwPacketNumber != laststate.dwPacketNumber)
		{
			laststate.Gamepad = eta->state->Gamepad;
			laststate.dwPacketNumber = eta->state->dwPacketNumber;

			if (eta->bind->GetStick() == LEFT_ST)
			{
				stateMTX.lock();
				ThumbX = float(eta->state->Gamepad.sThumbLX);
				ThumbY = float(eta->state->Gamepad.sThumbLY);
				stateMTX.unlock();
			}
			if (eta->bind->GetStick() == RIGHT_ST)
			{
				stateMTX.lock();
				ThumbX = float(eta->state->Gamepad.sThumbRX);
				ThumbY = float(eta->state->Gamepad.sThumbRY);
				stateMTX.unlock();
			}
		}

		float ChangeX = 0;
		float ChangeY = 0;

		float xPercent = (ThumbX / THUMBSTICK_MAX);
		float yPercent = (ThumbY / THUMBSTICK_MAX);

		ChangeX = ((xPercent)* CURSOR_MAX_SPEED);
		ChangeY = -((yPercent)* CURSOR_MAX_SPEED);

		input.mi.dx = (long)ChangeX;
		input.mi.dy = (long)ChangeY;

		if (int(sqrt((ThumbX*ThumbX) + (ThumbY*ThumbY))) > DEADZONE) //if the circular deadzone threshold is passed
		{
			SendInput(1, &input, sizeof(input));
		}
		Sleep(10);
	}
	return 0;
}

DWORD WINAPI tExecWheel(void* vETA)
{
	ExecThreadArgs* eta = ((ExecThreadArgs*)vETA);

	assert(eta != NULL);
	assert(eta->bind != NULL);

	XINPUT_STATE laststate;
	ZeroMemory(&laststate, sizeof(XINPUT_STATE));

	INPUT input;
	ZeroMemory(&input, sizeof(INPUT));

	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_WHEEL;

	printf("Starting Wheel Thread \n");

	DWORD AutorepeatTimer = 0;
	bool KeyDown = false;

	bool running = true;
	while (running)
	{
		stateMTX.lock();

		laststate.Gamepad = eta->state->Gamepad;
		laststate.dwPacketNumber = eta->state->dwPacketNumber;

		if ((eta->bind->GetButton() & eta->state->Gamepad.wButtons) != 0)
		{
			if (eta->bind->GetDir() == UP)
			{
				input.mi.mouseData = WHEEL_DELTA;
			}
			else
			{
				input.mi.mouseData = -WHEEL_DELTA;
			}

			if ((GetTickCount() - AutorepeatTimer) >= AUTOREPEAT_INTERVAL)
			{
				SendInput(1, &input, sizeof(input));
				AutorepeatTimer = GetTickCount();
			}
		}
		stateMTX.unlock();
		Sleep(THREAD_REST_TIME);
	}
	return 0;
}

DWORD WINAPI tExecTrigger(void* vETA)
{
	ExecThreadArgs* eta = ((ExecThreadArgs*)vETA);

	assert(eta != NULL);
	assert(eta->bind != NULL);

	XINPUT_STATE laststate;
	ZeroMemory(&laststate, sizeof(XINPUT_STATE));

	INPUT input;
	ZeroMemory(&input, sizeof(INPUT));

	int inputssent = 0;

	printf("Starting Trigger Thread \n");

	input.type = INPUT_KEYBOARD;
	input.ki.dwFlags = eta->bind->GetKeyFlags();
	input.ki.wVk = eta->bind->GetVK_Key();
	input.ki.wScan = eta->bind->GetSC_Key();

	BYTE* Trigger = NULL;

	if (eta->bind->GetButton() == LEFT_TRIGGER)
	{
		Trigger = &(eta->state->Gamepad.bLeftTrigger);
	}
	else if (eta->bind->GetButton() == RIGHT_TRIGGER)
	{
		Trigger = &(eta->state->Gamepad.bRightTrigger);
	}
	else
	{
		printf("Trigger Thread Failed, Invalid Trigger");
	}

	DWORD AutorepeatTimer = 0;
	bool KeyDown = false;
	bool NeedSendKeyUp = false;

	bool running = true;
	while (running)
	{
		stateMTX.lock();
		if (*Trigger < TRIGGER_THRESHOLD) // the trigger above the threshold
		{
			input.ki.dwFlags |= KEYEVENTF_KEYUP;
			KeyDown = false;
		}
		else //the trigger is past the threshold
		{
			input.ki.dwFlags &= ~KEYEVENTF_KEYUP;
			KeyDown = true;
			NeedSendKeyUp = true;
		}
		stateMTX.unlock();
		Sleep(THREAD_REST_TIME);

		if (KeyDown == false)
		{
			if (NeedSendKeyUp == true)
			{
				SendInput(1, &input, sizeof(input));
				NeedSendKeyUp = false;
			}
		}
		else if (KeyDown == true)
		{
			if ((GetTickCount() - AutorepeatTimer) >= AUTOREPEAT_INTERVAL)
			{
				SendInput(1, &input, sizeof(input));
				AutorepeatTimer = GetTickCount();
			}
		}
	}
	return 0;
}

bool SpawnExecThread(ExecThreadArgs& ETA)
{
	assert(ETA.bind != NULL);
	assert(ETA.state != NULL);

	if (ETA.bind->GetMode() == STANDARD) { HANDLE hStandardThread = CreateThread(NULL, 0, tExecStandard, (LPVOID)&ETA, 0, NULL); }
	else if (ETA.bind->GetMode() == TOGGLE)   { HANDLE hToggleThread = CreateThread(NULL, 0, tExecToggle, (LPVOID)&ETA, 0, NULL); }
	else if (ETA.bind->GetMode() == ST_DIR)   { HANDLE hDirThread = CreateThread(NULL, 0, tExecST_Dir, (LPVOID)&ETA, 0, NULL); }
	else if (ETA.bind->GetMode() == ST_MOUSE) { HANDLE hMouseThread = CreateThread(NULL, 0, tExecST_Mouse, (LPVOID)&ETA, 0, NULL); }
	else if (ETA.bind->GetMode() == WHEEL)    { HANDLE hWheelThread = CreateThread(NULL, 0, tExecWheel, (LPVOID)&ETA, 0, NULL); }
	else if (ETA.bind->GetMode() == TRIGGER)  { HANDLE hTriggerThread = CreateThread(NULL, 0, tExecTrigger, (LPVOID)&ETA, 0, NULL); }
	else
	{
		printf("Failed to spawn thread: Invalid Thread Type \n");
		return false;
	}
	return true;
}