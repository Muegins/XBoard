#include <stdafx.h>
#include <windows.h>

#include "ButtonBind.h"

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
