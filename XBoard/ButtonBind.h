#ifndef BUTTONBIND_H
#define BUTTONBIND_H

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

struct KeyDef
{
	char* KeyString;
	int mVK_Key;
	int mSC_Key;
	int mKeyFlags;
};

extern struct KeyDef KeyDefArray[];

class ButtonBind
{
public:

	ButtonBind() : mButton(0), mStick(0), mDir(0), mMouseKey(0), mKeyFlags(0), mVK_Key(0), mSC_Key(0), mMode(0), mTogState(0) {} //default constructor for the ButtonBind.
	
	ButtonBind(char* XBbutton, char* KBkey, char* Mode, char* Dir); //constructor used by the parser to create populated ButtonBinds. Creates a filled out bind based on data parsed from the config file.

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


#endif //!BUTTONBIND_H
