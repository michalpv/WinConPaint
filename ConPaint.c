#include <windows.h>
#include <stdio.h>
#include "Console.h"
#include "Graphics.h"

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
	
	struct MouseRecord prevMouseRecord = {0, 0, 0}; // Set X, Y, and buttonState to 0 (no mouse button down)
	
	while(1) {
		struct MouseRecord mouseRecord;
		if (!(res = GetMouseState(&mouseRecord, hConIn))) {
			if (!(mouseRecord.X == prevMouseRecord.X && mouseRecord.Y == prevMouseRecord.Y && mouseRecord.buttonState == prevMouseRecord.buttonState)) { // Conditional checking to make sure that current input is different from previous input
				// Check for buttonState
				if (mouseRecord.buttonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
					// Call graphics function to draw to screen
					DrawCell(mouseRecord.X + 1, mouseRecord.Y + 1, 0, 0, 255); // Draw
				}
				if (mouseRecord.buttonState == RIGHTMOST_BUTTON_PRESSED) {
					// Call graphics function to draw to screen
					DrawCell(mouseRecord.X + 1, mouseRecord.Y + 1, 0, 0, 0); // Erase
				}
				
				prevMouseRecord.X = mouseRecord.X;
				prevMouseRecord.Y = mouseRecord.Y;
				prevMouseRecord.buttonState = mouseRecord.buttonState;
			}
		}
		else {
			return res;
		}
	}
}