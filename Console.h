#ifndef Console_Defined
#define Console_Defined

#define ESC "\x1b"
#define CSI "\x1b["

DWORD printErr(char *message);
int ConsoleSetup(HANDLE hConIn, HANDLE hConOut);

struct MouseRecord {
	SHORT X;
	SHORT Y;
	DWORD buttonState;
};

// Function to get mouse position and button state
DWORD GetMouseState(struct MouseRecord*, HANDLE hConIn);
#endif