#include <windows.h>
#include <stdio.h>
#include "Console.h"

DWORD printErr(char *message) {
	DWORD lastErr = GetLastError();
	printf(ESC "[0;0H"); // Set cursor position to 0, 0
	printf("[-] %s; System error code: %u", message, lastErr);
	return lastErr;
}

int ConsoleSetup(HANDLE hConIn, HANDLE hConOut) {
	// SetConsoleMode with flags to enable virtual terminal sequences for output (as well as mouse input and regular input modes)

	// Get flags for input handle
	DWORD modeFlags = 0;
	if (!GetConsoleMode(hConIn, &modeFlags)) {
		printf("Failed to get console flags for input handle");
		return GetLastError();
	}

	modeFlags = ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS; // Add additional flags if necessary
	// Set the flags for input buffer:
	if (!SetConsoleMode(hConIn, modeFlags)) {
		printf("Failed to set input console flags");
		return GetLastError();
	}

	// Get flags for output handle
	if (!GetConsoleMode(hConOut, &modeFlags)) {
		printf("Failed to get console flags for output handle");
		return GetLastError();
	}

	modeFlags |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	// Set flags for output buffer:
	if (!SetConsoleMode(hConOut, modeFlags)) { // Add additional flags if necessary
		printf("Failed to set output console flags");
		return GetLastError();
	}
	
	printf(ESC "[?25l"); // Disable cursor blinking

	return 0;
}

DWORD GetMouseState(struct MouseRecord *mouseRecord, HANDLE hConIn) {
	INPUT_RECORD inputRecord;
	DWORD inputsRead;
	
	if(!ReadConsoleInput(hConIn, &inputRecord, 1, &inputsRead)) {
		return GetLastError();
	}
	if (inputRecord.EventType == MOUSE_EVENT) {
		mouseRecord->X = inputRecord.Event.MouseEvent.dwMousePosition.X;
		mouseRecord->Y = inputRecord.Event.MouseEvent.dwMousePosition.Y;
		mouseRecord->buttonState = inputRecord.Event.MouseEvent.dwButtonState; // https://docs.microsoft.com/en-us/windows/console/mouse-event-record-str
	}
	return 0;
}