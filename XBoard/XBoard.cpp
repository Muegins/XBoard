#include "stdafx.h"
#include <windows.h>
#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <fstream>
#include "math.h"
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <mutex>

#include "XBoard.h"

using namespace std;

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

		printf( "\nBind Contains: \n" );
		printf( "mButton = %i \n",TempBind->GetButton() );
		printf( "mStick = %i \n", TempBind->GetStick() );
		printf( "mDir = %i \n", TempBind->GetDir() );
		printf( "mMouseKey = %i \n", TempBind->GetMouseKey() );
		printf( "mKeyFlags = %i \n", TempBind->GetKeyFlags() );
		printf( "mVK_Key = 0x%x \n", TempBind->GetVK_Key() );
		printf( "mSC_Key = 0x%x \n", TempBind->GetSC_Key() );
		printf( "mMode = %i \n", TempBind->GetMode() );
		printf( "mTogState = %i \n", TempBind->GetTogState() );
		printf("\n");

		assert(GetArrayPosition( TempBind->GetButton(), TempBind->GetDir() ) < MAX_BINDS);
		ThreadDataArray[GetArrayPosition(TempBind->GetButton(), TempBind->GetDir())].bind = TempBind;

		k++;
	}
	free(pc);
	return lines;
}

