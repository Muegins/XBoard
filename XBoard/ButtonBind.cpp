#include <stdafx.h>
#include <Windows.h>
#include <XInput.h>

#include "Misc.h"
#include "ButtonBind.h"

ButtonBind::ButtonBind(char* XBbutton, char* KBkey, char* Mode, char* Dir)
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