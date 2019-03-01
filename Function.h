#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <windows.h>
using namespace std;
#pragma warning(disable:4996)
typedef BOOL(WINAPI *SETCONSOLEFONT)(HANDLE, DWORD);
typedef BOOL(WINAPI *SETCURRENTCONSOLEFONTEX)(HANDLE, BOOL, PCONSOLE_FONT_INFOEX);
SETCONSOLEFONT SetConsoleFont;

void start(int row, int column) {
	OSVERSIONINFOEX osvi;
	CONSOLE_FONT_INFOEX cfon;
	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	GetVersionEx((OSVERSIONINFO *)&osvi);
	HMODULE hmod = GetModuleHandleA("KERNEL32.DLL");
	if (osvi.dwMajorVersion<6) {
		SetConsoleFont = (SETCONSOLEFONT)GetProcAddress(hmod, "SetConsoleFont");
		if (!SetConsoleFont) { exit(1); }
		SetConsoleFont(GetStdHandle(STD_OUTPUT_HANDLE), 10);
	}
	else
	{
		if (!SetCurrentConsoleFontEx) { exit(1); }
		ZeroMemory(&cfon, sizeof(CONSOLE_FONT_INFOEX));

		cfon.cbSize = sizeof(CONSOLE_FONT_INFOEX);
		cfon.nFont = 5;
		cfon.dwFontSize.X = 15;
		cfon.dwFontSize.Y = 15;
		cfon.FontFamily = 0;
		SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), 0, &cfon);
	}
	char buf[256] = { 0 };
	sprintf(buf, "mode con cols=%d lines=%d", column, row);
	system(buf);
	system("cls");
}

enum DIR {
	FALL, LEFT, RIGHT, UP, DOWN, QUIT, CHANGE
};

#ifdef _UP 
#undef _UP
#endif

enum KEYS {
	ESC = 27,
	ENTER = 13,
	SPACE = 32,
	CURSOR1 = 0, CURSOR2 = 224,
	_LEFT = 75,
	_RIGHT = 77,
	_UP = 72,
	_DOWN = 80,
	X = 'X', _X = 'x'
};

void SetColor(WORD wAttributes)
{
	HANDLE hOUTPUT = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hOUTPUT, wAttributes);
}
void SetColor(int bkgr, int sym) {
	SetColor((bkgr << 4) | sym);
}

void SetPos(int x, int y) {
	COORD cd;
	cd.X = x;
	cd.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cd);
}

void CursorHide() {
	HANDLE hOUTPUT = GetStdHandle(STD_OUTPUT_HANDLE);
	hOUTPUT = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo;
	cursorInfo.dwSize = 1;
	cursorInfo.bVisible = 0;
	SetConsoleCursorInfo(hOUTPUT, &cursorInfo);
}