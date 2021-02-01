#include <windows.h>
#include <stdio.h>
#include "Console.h"

void DrawCell(SHORT X, SHORT Y, unsigned char r, unsigned char g, unsigned char b) {
	printf(ESC "[%d;%dH", Y, X); // Set cursor position to X and Y
	printf(ESC "[48;2;%d;%d;%dm  ", r, g ,b); // Set colour of cell and print a space (colour currently hardcoded)
	printf(ESC "[48;2;0;0;0m");
	return;
}

void ClearCanvas() {
	printf(ESC "[0;0H"); // Set cursor to 0, 0
	printf(ESC "[2J"); // Clear scroll
	printf(ESC "[3J"); // Clear scroll back
	return;
}