// HidTerm.cpp : Defines the entry point for the console application.
//

#include <windows.h>

#include <stdio.h>
#include <tchar.h>

#include "Port.h"

#define InputBuffersCount 128

void CheckKeyboard(HANDLE readConsole, char *characters, size_t *count)
{
	DWORD numberOfEvents;
	GetNumberOfConsoleInputEvents(readConsole, &numberOfEvents);

	if(numberOfEvents > 0) {
		INPUT_RECORD inputBuffers[InputBuffersCount];
		DWORD numberOfEventsRead;

		if(!ReadConsoleInput(readConsole, inputBuffers, InputBuffersCount, &numberOfEventsRead))
			return;

		for(DWORD i = 0; i < numberOfEventsRead; i++) {
			if(inputBuffers[i].EventType == KEY_EVENT && 
				inputBuffers[i].Event.KeyEvent.bKeyDown)
			{
				if(inputBuffers[i].Event.KeyEvent.uChar.AsciiChar != 0x00) {
					characters[*count] = inputBuffers[i].Event.KeyEvent.uChar.AsciiChar;
					(*count)++;

					printf("[%c]", inputBuffers[i].Event.KeyEvent.uChar.AsciiChar);
				}
			}
		}
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	HANDLE readConsole = GetStdHandle(STD_INPUT_HANDLE);
	SetConsoleTitle(_T("HID Terminal"));

	Hid hid(0x16C0, 0x05DC);
	if(!hid.IsOpen()) {
		printf("Could not open the port\n");
		getchar();
		return(1);
	}

	bool lastWasCR = false;
	while(hid.IsOpen()) {
		char charactersToSend[InputBuffersCount];
		size_t charactersToSendCount = 0;

		CheckKeyboard(readConsole, charactersToSend, &charactersToSendCount);
		if(charactersToSendCount > 0)
			if(!hid.Send(charactersToSend, charactersToSendCount))
				printf("[Could not send!]");
		
		while(hid.CanRead()) {
			char ch;
			hid.ReceiveByte(ch);

			if(ch == '\n') {
				printf("\n");
				lastWasCR = false;
			} else if(ch == '\r' && !lastWasCR) {
				lastWasCR = true;
			} else {
				if(lastWasCR) // if we had an CR but that was not followed by a LF
					printf("(%02x)", '\r');

				if(ch < 0x20)
					printf("(%02x)", ch & 0xff);
				else
					putchar(ch);
			}
			


		}
	}

	return 0;
}

