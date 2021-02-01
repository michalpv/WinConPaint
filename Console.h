#ifndef Console_Defined
#define Console_Defined

#define ESC "\x1b"
#define CSI "\x1b["

DWORD printErr(char *message);
int ConsoleSetup(HANDLE hConIn, HANDLE hConOut);

#endif