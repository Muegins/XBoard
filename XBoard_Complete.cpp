#include "stdafx.h"
#include <windows.h>
#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <fstream>
#include "XInput.h"
#include "math.h"
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <mutex>

using namespace std;

#define MAX_BINDS 28

#define STANDARD 10
#define TOGGLE   20
#define ST_DIR   30
#define ST_MOUSE 40
#define WHEEL    50
#define TRIGGER  60

#define LEFT_ST  15
#define RIGHT_ST 25
#define LEFT_TRIGGER 35
#define RIGHT_TRIGGER 45

#define UP    100
#define DOWN  200
#define LEFT  300
#define RIGHT 400

#define CURSOR_MAX_SPEED 10 //Measured in pixels/10 millis
#define THUMBSTICK_MAX 32767 //Maximum value of thumbstick output
#define DEADZONE 10000 //Measured in same units as thumbstick
#define AUTOREPEAT_INTERVAL 20 //millis
#define TRIGGER_THRESHOLD 15 //min 0(unpressed) max 255(fully pressed)
#define THREAD_REST_TIME 10 //millis


#define MBUTTON		 5
#define MSTICK		15
#define MDIR		25
#define MMOUSEKEY	35
#define MKeyFlags		45
#define MSC_KEY		55
#define MVK_KEY		65
#define MMODE		75
#define MTOGSTATE	85

mutex stateMTX;

struct KeyDef
{
	char* KeyString;
	int mVK_Key;
	int mSC_Key;
	int mKeyFlags;
};

#define GAMEPAD_A 0
#define GAMEPAD_B 1
#define GAMEPAD_Y 2
#define GAMEPAD_X 3
#define GAMEPAD_DPAD_UP 4
#define GAMEPAD_DPAD_DOWN 5
#define GAMEPAD_DPAD_LEFT 6
#define GAMEPAD_DPAD_RIGHT 7
#define GAMEPAD_LEFT_SHOULDER 8
#define GAMEPAD_RIGHT_SHOULDER 9
#define GAMEPAD_LEFT_THUMB 10
#define GAMEPAD_RIGHT_THUMB 11
#define GAMEPAD_START 12
#define GAMEPAD_BACK 13
#define GAMEPAD_LEFT_ST_UP 14
#define GAMEPAD_LEFT_ST_DOWN 15
#define GAMEPAD_LEFT_ST_LEFT 16
#define GAMEPAD_LEFT_ST_RIGHT 17
#define GAMEPAD_RIGHT_ST_UP 18
#define GAMEPAD_RIGHT_ST_DOWN 19
#define GAMEPAD_RIGHT_ST_LEFT 20
#define GAMEPAD_RIGHT_ST_RIGHT 21
#define GAMEPAD_LEFT_ST_MOUSE 22
#define GAMEPAD_RIGHT_ST_MOUSE 23
#define LEFT_TRIGGER_POS 24
#define RIGHT_TRIGGER_POS 25
//#define 26
//#define 27

#define A 0
#define B 1
#define C 2
#define D 3
#define E 4
#define F 5
#define G 6
#define H 7
#define I 8
#define J 9
#define K 10
#define L 11
#define M 12
#define N 13
#define O 14
#define P 15
#define Q 16
#define R 17
#define S 18
#define T 19
#define U 20
#define V 21
#define W 22
#define X 23
#define Y 24
#define Z 25

#define LEFT_MOUSE 26
#define RIGHT_MOUSE 27
#define MIDDLE_MOUSE 28

#define SPACE 29

struct KeyDef KeyDefArray[] =
{
	//  {"Key String", VK  , SC  , EX   }  //Example

	//  Mouse Buttons
	{ "LEFT_MOUSE", 0x01, MapVirtualKey(0x01, MAPVK_VK_TO_VSC), NULL },
	{ "RIGHT_MOUSE", 0x02, MapVirtualKey(0x02, MAPVK_VK_TO_VSC), NULL },
	{ "MIDDLE_MOUSE", 0x04, MapVirtualKey(0x04, MAPVK_VK_TO_VSC), NULL },

	//  Number Keys
	{ "NK_0", 0x30, 0x45, NULL },
	{ "NK_1", 0x31, 0x16, NULL },
	{ "NK_2", 0x32, 0x1E, NULL },
	{ "NK_3", 0x33, 0x26, NULL },
	{ "NK_4", 0x34, 0x25, NULL },
	{ "NK_5", 0x35, 0x2E, NULL },
	{ "NK_6", 0x36, 0x36, NULL },
	{ "NK_7", 0x37, 0x3D, NULL },
	{ "NK_8", 0x38, 0x3E, NULL },
	{ "NK_9", 0x39, 0x46, NULL },

	//  Letter Keys
	{ "A", 0x41, 0x1E, NULL },
	{ "B", 0x42, 0x30, NULL },
	{ "C", 0x43, 0x2E, NULL },
	{ "D", 0x44, 0x20, NULL },
	{ "E", 0x45, 0x12, NULL },
	{ "F", 0x46, 0x21, NULL },
	{ "G", 0x47, 0x22, NULL },
	{ "H", 0x48, 0x23, NULL },
	{ "I", 0x49, 0x17, NULL },
	{ "J", 0x4A, 0x24, NULL },
	{ "K", 0x4B, 0x25, NULL },
	{ "L", 0x4C, 0x26, NULL },
	{ "M", 0x4D, 0x32, NULL },
	{ "N", 0x4E, 0x31, NULL },
	{ "O", 0x4F, 0x18, NULL },
	{ "P", 0x50, 0x19, NULL },
	{ "Q", 0x51, 0x10, NULL },
	{ "R", 0x52, 0x13, NULL },
	{ "S", 0x53, 0x1F, NULL },
	{ "T", 0x54, 0x14, NULL },
	{ "U", 0x55, 0x16, NULL },
	{ "V", 0x56, 0x2F, NULL },
	{ "W", 0x57, 0x11, NULL },
	{ "X", 0x58, 0x2D, NULL },
	{ "Y", 0x59, 0x15, NULL },
	{ "Z", 0x5A, 0x2C, NULL },

	//  NumPad Keys 
	{ "NP_0", 0x60, 0x70, NULL },
	{ "NP_1", 0x61, 0x69, NULL },
	{ "NP_2", 0x62, 0x72, NULL },
	{ "NP_3", 0x63, 0x7A, NULL },
	{ "NP_4", 0x64, 0x6B, NULL },
	{ "NP_5", 0x65, 0x73, NULL },
	{ "NP_6", 0x66, 0x74, NULL },
	{ "NP_7", 0x67, 0x6C, NULL },
	{ "NP_8", 0x68, 0x75, NULL },
	{ "NP_9", 0x69, 0x73, NULL },

	//  Function keys
	{ "FN_1", 0x70, 0x05, NULL },
	{ "FN_2", 0x71, 0x06, NULL },
	{ "FN_3", 0x72, 0x04, NULL },
	{ "FN_4", 0x73, 0x0C, NULL },
	{ "FN_5", 0x74, 0x03, NULL },
	{ "FN_6", 0x75, 0x0B, NULL },
	{ "FN_7", 0x76, 0x83, NULL },
	{ "FN_8", 0x77, 0x0A, NULL },
	{ "FN_9", 0x78, 0x01, NULL },
	{ "FN_10", 0x79, 0x09, NULL },
	{ "FN_11", 0x7A, 0x78, NULL },
	{ "FN_12", 0x7B, 0x07, NULL },

	//  Arrow Keys
	{ "LEFT_ARROW", 0x25, 0x6B, KEYEVENTF_EXTENDEDKEY },
	{ "UP_ARROW", 0x26, 0x75, KEYEVENTF_EXTENDEDKEY },
	{ "RIGHT_ARROW", 0x27, 0x74, KEYEVENTF_EXTENDEDKEY },
	{ "DOWN_ARROW", 0x28, 0x72, KEYEVENTF_EXTENDEDKEY },

	//  Special Keys	
	{ "BACKSPACE", 0x08, 0x66, NULL },
	{ "TAB", 0x09, 0x0D, NULL },
	{ "SHIFT", 0x10, 0x12, NULL },
	{ "CTRL", 0x11, 0x14, NULL },
	{ "ALT", 0x12, 0x11, NULL },
	{ "PAUSE", 0x13, MapVirtualKey(0x13, MAPVK_VK_TO_VSC), NULL },
	{ "CAPS", 0x14, 0x58, NULL },
	{ "SPACE", 0x20, 0x39, NULL },
	{ "PAGE_UP", 0x21, 0x7D, KEYEVENTF_EXTENDEDKEY },
	{ "PAGE_DOWN", 0x22, 0x7A, KEYEVENTF_EXTENDEDKEY },
	{ "ENTER", 0x0D, 0x5A, NULL },
	{ "ESC", 0x1B, 0x76, NULL },
	{ "END", 0x23, 0x69, KEYEVENTF_EXTENDEDKEY },
	{ "HOME", 0x24, 0x6C, KEYEVENTF_EXTENDEDKEY },
	{ "INSERT", 0x2D, 0x70, KEYEVENTF_EXTENDEDKEY },
	{ "DELETE", 0x2E, 0x71, KEYEVENTF_EXTENDEDKEY },

	//  OEM keys (OEMs have no common VK's and use only scans. KEYEVENTF_SCANCODE should be flagged for each)	
	{ "`", NULL, 0xE0, KEYEVENTF_SCANCODE },
	{ "-", NULL, 0x4E, KEYEVENTF_SCANCODE },
	{ "=", NULL, 0x55, KEYEVENTF_SCANCODE },
	{ "\\", NULL, 0x5D, KEYEVENTF_SCANCODE }, //Key String in config should be "\"
	{ "[", NULL, 0x54, KEYEVENTF_SCANCODE },
	{ "]", NULL, 0x5B, KEYEVENTF_SCANCODE },
	{ ";", NULL, 0x4C, KEYEVENTF_SCANCODE },
	{ " ' ", NULL, 0x52, KEYEVENTF_SCANCODE },
	{ ",", NULL, 0x41, KEYEVENTF_SCANCODE },
	{ ".", NULL, 0x49, KEYEVENTF_SCANCODE },
	{ "/", NULL, 0x4A, KEYEVENTF_SCANCODE },

	//  System 
	{ "NULL", 0, 0, NULL },
	{ "LAST", 0, 0, NULL }
};

class ButtonBind
{
public:


	ButtonBind() : mButton(0), mStick(0), mDir(0), mMouseKey(0), mKeyFlags(0), mVK_Key(0), mSC_Key(0), mMode(0), mTogState(0) {}

	ButtonBind(char* XBbutton, char* KBkey, char* Mode, char* Dir)
	{
		bool searching = true;
		int i = 0;

		while (searching)
		{
			if (strcmp(KBkey, KeyDefArray[i].KeyString) == 0)
			{
				mKeyFlags = KeyDefArray[i].mKeyFlags;
				mSC_Key = KeyDefArray[i].mSC_Key;
				mVK_Key = KeyDefArray[i].mVK_Key;
				searching = false;
			}
			else if (strcmp(KeyDefArray[i].KeyString, "LAST") == 0)
			{
				printf("Error creating ButtonBind, \"%s\" is not a valid key.\n", KBkey);
				mKeyFlags = 0;
				mSC_Key = 0;
				mSC_Key = 0;
				searching = false;
			}
			i++;
		}

		if (Dir == NULL) { mDir = 0; }
		else if (!strcmp(Dir, "UP")) { mDir = UP; }
		else if (!strcmp(Dir, "DOWN")) { mDir = DOWN; }
		else if (!strcmp(Dir, "LEFT")) { mDir = LEFT; }
		else if (!strcmp(Dir, "RIGHT")) { mDir = RIGHT; }
		else
		{
			printf("Error creating ButtonBind, \"%s\" is not a valid direction.\n", Dir);
			mDir = 0;
		}
		if (!strcmp(Mode, "STANDARD")) { mMode = STANDARD; mMouseKey = INPUT_KEYBOARD; }
		else if (!strcmp(Mode, "TOGGLE"))   { mMode = TOGGLE; mMouseKey = INPUT_KEYBOARD; }
		else if (!strcmp(Mode, "ST_DIR"))   { mMode = ST_DIR; mMouseKey = INPUT_KEYBOARD; }
		else if (!strcmp(Mode, "ST_MOUSE")) { mMode = ST_MOUSE; mMouseKey = INPUT_MOUSE; }
		else if (!strcmp(Mode, "WHEEL"))    { mMode = WHEEL; mMouseKey = INPUT_MOUSE; }
		else if (!strcmp(Mode, "TRIGGER"))  { mMode = TRIGGER; mMouseKey = INPUT_KEYBOARD; }
		else
		{
			printf("Error creating ButtonBind, \"%s\" is not a valid mode.\n", Mode);
			mMode = 0;
			mMouseKey = 0;
		}

		switch (mMode)
		{
		case STANDARD:
		case TOGGLE:
		case WHEEL:
		case TRIGGER:
		{
			mStick = 0;

			if (!strcmp(XBbutton, "XINPUT_GAMEPAD_DPAD_UP"))		 { mButton = XINPUT_GAMEPAD_DPAD_UP; }
			else if (!strcmp(XBbutton, "XINPUT_GAMEPAD_DPAD_DOWN"))		 { mButton = XINPUT_GAMEPAD_DPAD_DOWN; }
			else if (!strcmp(XBbutton, "XINPUT_GAMEPAD_DPAD_LEFT"))		 { mButton = XINPUT_GAMEPAD_DPAD_LEFT; }
			else if (!strcmp(XBbutton, "XINPUT_GAMEPAD_DPAD_RIGHT"))	 { mButton = XINPUT_GAMEPAD_DPAD_RIGHT; }
			else if (!strcmp(XBbutton, "XINPUT_GAMEPAD_START"))			 { mButton = XINPUT_GAMEPAD_START; }
			else if (!strcmp(XBbutton, "XINPUT_GAMEPAD_BACK"))			 { mButton = XINPUT_GAMEPAD_BACK; }
			else if (!strcmp(XBbutton, "XINPUT_GAMEPAD_LEFT_THUMB"))	 { mButton = XINPUT_GAMEPAD_LEFT_THUMB; }
			else if (!strcmp(XBbutton, "XINPUT_GAMEPAD_RIGHT_THUMB"))	 { mButton = XINPUT_GAMEPAD_RIGHT_THUMB; }
			else if (!strcmp(XBbutton, "XINPUT_GAMEPAD_LEFT_SHOULDER"))  { mButton = XINPUT_GAMEPAD_LEFT_SHOULDER; }
			else if (!strcmp(XBbutton, "XINPUT_GAMEPAD_RIGHT_SHOULDER")) { mButton = XINPUT_GAMEPAD_RIGHT_SHOULDER; }
			else if (!strcmp(XBbutton, "XINPUT_GAMEPAD_A"))				 { mButton = XINPUT_GAMEPAD_A; }
			else if (!strcmp(XBbutton, "XINPUT_GAMEPAD_B"))				 { mButton = XINPUT_GAMEPAD_B; }
			else if (!strcmp(XBbutton, "XINPUT_GAMEPAD_X"))				 { mButton = XINPUT_GAMEPAD_X; }
			else if (!strcmp(XBbutton, "XINPUT_GAMEPAD_Y"))				 { mButton = XINPUT_GAMEPAD_Y; }
			else if (!strcmp(XBbutton, "LEFT_TRIGGER"))				     { mButton = LEFT_TRIGGER; }
			else if (!strcmp(XBbutton, "RIGHT_TRIGGER"))				 { mButton = RIGHT_TRIGGER; }
			else
			{
				printf("Error creating ButtonBind, \"%s\" is not a valid button type.\n", XBbutton);
				mButton = 0;
			}
			break;
		}
		case ST_DIR:
		case ST_MOUSE:
		{
			mButton = 0;

			if (!strcmp(XBbutton, "LEFT_ST"))  { mStick = LEFT_ST; mButton = LEFT_ST; }
			else if (!strcmp(XBbutton, "RIGHT_ST")) { mStick = RIGHT_ST; mButton = RIGHT_ST; }
			else
			{
				printf("Error creating ButtonBind, \"%s\" is not a valid stick.\n", XBbutton);
				mStick = 0;
			}
		}
		}
	}

	int GetBindMember(int memberdef)
	{
		if (MBUTTON)    { return mButton; }
		else if (MSTICK)     { return mStick; }
		else if (MDIR)		 { return mDir; }
		else if (MMOUSEKEY)  { return mMouseKey; }
		else if (MKeyFlags)     { return mKeyFlags; }
		else if (MVK_KEY)    { return mVK_Key; }
		else if (MSC_KEY)    { return mSC_Key; }
		else if (MMODE)      { return mMode; }
		else if (MTOGSTATE)  { return mTogState; }
		else
		{
			printf("Argument is not a valid member of ButtonBind");
			return -1;
		}
	}

	int GetButton()
	{
		return mButton;
	}

	int GetStick()
	{
		return mStick;
	}

	int GetDir()
	{
		return mDir;
	}

	int GetMouseKey()
	{
		return mMouseKey;
	}

	int GetKeyFlags()
	{
		return mKeyFlags;
	}

	int GetVK_Key()
	{
		return mVK_Key;
	}

	int GetSC_Key()
	{
		return mSC_Key;
	}

	int GetMode()
	{
		return mMode;
	}

	bool GetTogState()
	{
		return mTogState;
	}

	void SetTogState(bool val)
	{
		mTogState = val;
	}
protected:

	int mButton;        // Stores the XBOX button\s. (XINPUT_GAMEPAD_FOO, (XINPUT_GAMEPAD_FOO & XINPUT_GAMEPAD_BAZ) )
	int mStick;         // Stores the stick being used. (LEFT_ST, RIGHT_ST)
	int mDir;           // Stores the direction of being bound for ST_DIR and WHEEL binds. (UP, DOWN, LEFT, RIGHT)
	int mMouseKey;      // Tells if action is mouse or keyboard action. Mouse buttons presses are treated as keyboard actions, wheel rolls are mouse actions. (INPUT_MOUSE, INPUT_KEYBOARD, INPUT_HARDWARE)
	int mKeyFlags;		// Holds flags for input structure Mulitple flages can be |. (KEYEVENTF_EXTENDEDKEY) Tells if the key is in the extended set; scan preceded by E0. (KEYEVENTF_SCANCODE) Tells SendInput to ignore the VK code, used for OEM keys since they down have VKs.
	int mVK_Key;        // Stores keyboard Virtual Key code.
	int mSC_Key;        // Stores keyboard Scan Code.
	int mMode;	        // Stores the type of action bound. This determines which Exec fucntion is called (STANDARD, TOGGLE, ST_DIR, ST_MOUSE, WHEEL, TRIGGER)
	bool mTogState = 0; // Holds the state of a toggle.
};

struct ExecThreadArgs
{
	ButtonBind* bind;
	XINPUT_STATE* state;
	INPUT* InptArrayEntry;
};

int GetArrayPosition(int XBbutton, int Dir)
{
	int ArrayPosition = 0;
	switch (XBbutton)
	{
	case XINPUT_GAMEPAD_DPAD_UP:
	{
		ArrayPosition = GAMEPAD_DPAD_UP;
		break;
	}
	case XINPUT_GAMEPAD_DPAD_DOWN:
	{
		ArrayPosition = GAMEPAD_DPAD_DOWN;
		break;
	}
	case XINPUT_GAMEPAD_DPAD_LEFT:
	{
		ArrayPosition = GAMEPAD_DPAD_LEFT;
		break;
	}
	case XINPUT_GAMEPAD_DPAD_RIGHT:
	{
		ArrayPosition = GAMEPAD_DPAD_RIGHT;
		break;
	}
	case XINPUT_GAMEPAD_A:
	{
		ArrayPosition = GAMEPAD_A;
		break;
	}
	case XINPUT_GAMEPAD_B:
	{
		ArrayPosition = GAMEPAD_B;
		break;
	}
	case XINPUT_GAMEPAD_Y:
	{
		ArrayPosition = GAMEPAD_Y;
		break;
	}
	case XINPUT_GAMEPAD_X:
	{
		ArrayPosition = GAMEPAD_X;
		break;
	}
	case XINPUT_GAMEPAD_RIGHT_SHOULDER:
	{
		ArrayPosition = GAMEPAD_RIGHT_SHOULDER;
		break;
	}
	case XINPUT_GAMEPAD_LEFT_SHOULDER:
	{
		ArrayPosition = GAMEPAD_LEFT_SHOULDER;
		break;
	}
	case XINPUT_GAMEPAD_RIGHT_THUMB:
	{
		ArrayPosition = GAMEPAD_RIGHT_THUMB;
		break;
	}
	case XINPUT_GAMEPAD_LEFT_THUMB:
	{
		ArrayPosition = GAMEPAD_LEFT_THUMB;
		break;
	}
	case XINPUT_GAMEPAD_START:
	{
		ArrayPosition = GAMEPAD_START;
		break;
	}
	case XINPUT_GAMEPAD_BACK:
	{
		ArrayPosition = GAMEPAD_BACK;
		break;
	}
	case LEFT_TRIGGER:
	{
		ArrayPosition = LEFT_TRIGGER_POS;
		break;
	}
	case RIGHT_TRIGGER:
	{
		ArrayPosition = RIGHT_TRIGGER_POS;
		break;
	}
	case LEFT_ST:
	{
		switch (Dir)
		{
		case UP:
		{
			ArrayPosition = GAMEPAD_LEFT_ST_UP;
			break;
		}
		case DOWN:
		{
			ArrayPosition = GAMEPAD_LEFT_ST_DOWN;
			break;
		}
		case LEFT:
		{
			ArrayPosition = GAMEPAD_LEFT_ST_LEFT;
			break;
		}
		case RIGHT:
		{
			ArrayPosition = GAMEPAD_LEFT_ST_RIGHT;
			break;
		}
		default:
		{
			ArrayPosition = GAMEPAD_LEFT_ST_MOUSE;
			break;
		}
		}
		break;
	}
	case RIGHT_ST:
	{
		switch (Dir)
		{
		case UP:
		{
			ArrayPosition = GAMEPAD_RIGHT_ST_UP;
			break;
		}
		case DOWN:
		{
			ArrayPosition = GAMEPAD_RIGHT_ST_DOWN;
			break;
		}
		case LEFT:
		{
			ArrayPosition = GAMEPAD_RIGHT_ST_LEFT;
			break;
		}
		case RIGHT:
		{
			ArrayPosition = GAMEPAD_RIGHT_ST_RIGHT;
			break;
		}
		default:
		{
			ArrayPosition = GAMEPAD_RIGHT_ST_MOUSE;
			break;
		}
		}
		break;
	}
	default:
	{
		printf("Could not get bind array position: Invalid Button Value \n");
		ArrayPosition = -1;
	}
	}
	return ArrayPosition;
}

int ParseConfigFile(ExecThreadArgs* ThreadDataArray)
{
	FILE* fp;
	int size = 0;
	int lines = 1;
	fp = fopen("C:\\XBoardConfig\\config.txt", "rb");
	char* Ltokens[MAX_BINDS];

	if (fp == NULL)
	{
		printf("Failed to Open File \n");
		return -1;
	}

	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	//printf("size of file: %i \n", size);

	char* pc = (char*)malloc(size + 1);

	fread(pc, 1, (size + 1), fp);
	pc[size] = '\0';
	printf("%s \n", pc);

	Ltokens[0] = strtok(pc, "\n\r");
	while ((Ltokens[lines] = strtok(NULL, "\n\r")) != NULL)
	{
		printf("lines = %d \n", lines);
		printf("Ltokens[lines] = %s \n", Ltokens[lines]);
		lines = lines + 1;
	}
	int k = 0;
	while (k < lines)
	{
		char* XBbutton = strtok(Ltokens[k], " ");
		char* VKcode = strtok(NULL, " ");
		char* Mode = strtok(NULL, " ");
		char* Dir = strtok(NULL, " ");

		printf("XBbutton = %s \n", XBbutton);
		printf("VKcode = %s \n", VKcode);
		printf("Mode = %s \n", Mode);
		printf("Dir = %s \n", Dir);

		ButtonBind* TempBind = new ButtonBind(XBbutton, VKcode, Mode, Dir);

		printf("\nBind Contains: \n");
		printf("mButton = %i \n", TempBind->GetButton());
		printf("mStick = %i \n", TempBind->GetStick());
		printf("mDir = %i \n", TempBind->GetDir());
		printf("mMouseKey = %i \n", TempBind->GetMouseKey());
		printf("mKeyFlags = %i \n", TempBind->GetKeyFlags());
		printf("mVK_Key = 0x%x \n", TempBind->GetVK_Key());
		printf("mSC_Key = 0x%x \n", TempBind->GetSC_Key());
		printf("mMode = %i \n", TempBind->GetMode());
		printf("mTogState = %i \n", TempBind->GetTogState());
		printf("\n");

		assert(GetArrayPosition(TempBind->GetButton(), TempBind->GetDir()) < MAX_BINDS);
		ThreadDataArray[GetArrayPosition(TempBind->GetButton(), TempBind->GetDir())].bind = TempBind;

		k++;
	}
	free(pc);
	return lines;
}

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
			NeedSendKeyUp == true;
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