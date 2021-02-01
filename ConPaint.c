#include <Windows.h>
#include <stdio.h>
#include "Console.h"
#include "Graphics.h"

#pragma comment(lib, "Comdlg32.lib")

int main() {
	// Setup console for VT sequences and mouse input
	HANDLE hConIn;
	HANDLE hConOut;
	hConIn = GetStdHandle(STD_INPUT_HANDLE);
	hConOut = GetStdHandle(STD_OUTPUT_HANDLE);

	if (hConIn == INVALID_HANDLE_VALUE || hConOut == INVALID_HANDLE_VALUE) {
		printErr("Failed to obtain console handles");
	}
	
	DWORD res = 0;
	if (res = ConsoleSetup(hConIn, hConOut)) {
		return res; // GetLastError() returned by ConsoleSetup()
	}
	
	// Only after this point is it safe to use VT sequences
	
	MOUSE_EVENT_RECORD prevMouseRecord;
	COLORREF rgbCurrent = 0xFF0000; // 0x00bbggrr - https://docs.microsoft.com/en-us/windows/win32/gdi/colorref
	static COLORREF acrCustClr[16]; // array of custom colors for ChooseColor function
	
	while(1) {
		INPUT_RECORD inputRecord;
		DWORD inputsRead;
	
		if(!ReadConsoleInput(hConIn, &inputRecord, 1, &inputsRead)) {
			printErr("ReadConsoleInput failed");
		}
		else {
			switch(inputRecord.EventType) { // Input handler
				case MOUSE_EVENT:
					if (!(inputRecord.Event.MouseEvent.dwMousePosition.X == prevMouseRecord.dwMousePosition.X &&
						inputRecord.Event.MouseEvent.dwMousePosition.Y == prevMouseRecord.dwMousePosition.Y &&
						inputRecord.Event.MouseEvent.dwButtonState == prevMouseRecord.dwButtonState)) { // Check that mouse record is different before acting
						
						if (inputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
							// Call graphics function to draw to screen
							DrawCell(inputRecord.Event.MouseEvent.dwMousePosition.X + 1, inputRecord.Event.MouseEvent.dwMousePosition.Y + 1, (rgbCurrent & 0xFF), (rgbCurrent >> 8) & 0xFF, (rgbCurrent >> 16)); // Draw
						}
						else if (inputRecord.Event.MouseEvent.dwButtonState == RIGHTMOST_BUTTON_PRESSED) {
							// Call graphics function to draw to screen
							DrawCell(inputRecord.Event.MouseEvent.dwMousePosition.X + 1, inputRecord.Event.MouseEvent.dwMousePosition.Y + 1, 0, 0, 0); // Erase
						}
						
						// Set prevMouseRecord values to new mouse event record
						/*
						typedef struct _MOUSE_EVENT_RECORD {
							COORD dwMousePosition;
							DWORD dwButtonState;
							DWORD dwControlKeyState;
							DWORD dwEventFlags;
						} MOUSE_EVENT_RECORD;
						*/
						
						prevMouseRecord.dwMousePosition.X = inputRecord.Event.MouseEvent.dwMousePosition.X;
						prevMouseRecord.dwMousePosition.Y = inputRecord.Event.MouseEvent.dwMousePosition.Y;
						prevMouseRecord.dwButtonState = inputRecord.Event.MouseEvent.dwButtonState;
						prevMouseRecord.dwControlKeyState = inputRecord.Event.MouseEvent.dwControlKeyState;
						prevMouseRecord.dwEventFlags = inputRecord.Event.MouseEvent.dwEventFlags;
					}
					break;
				case KEY_EVENT:
						// Check that the key is down first:
						if (inputRecord.Event.KeyEvent.bKeyDown) {
							switch (inputRecord.Event.KeyEvent.uChar.AsciiChar) {
								case 0x63: { // "c"
									ClearCanvas();
									break;
								}
								case 0x78: { // "x", color selector
									CHOOSECOLOR cc;

									// Initialize CHOOSECOLOR 
									//memset(&cc, 0, sizeof(cc));
									ZeroMemory(&cc, sizeof(cc));
									cc.lStructSize = sizeof(cc);
									//cc.hwndOwner = NULL;
									cc.lpCustColors = (LPDWORD) acrCustClr;
									cc.rgbResult = rgbCurrent;
									cc.Flags = CC_FULLOPEN | CC_RGBINIT;
									
									if (ChooseColor(&cc)){
										rgbCurrent = cc.rgbResult;
									}
									break;
								}
							}
						}
					break;
			}
		}
	}
}