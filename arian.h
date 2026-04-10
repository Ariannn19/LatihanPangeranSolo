#ifndef ARIAN_H
#define ARIAN_H

#include <windows.h>

#define MAX_LINES 100
#define MAX_LENGTH 100

extern int cursorX, cursorY;
extern int lines;
extern int idealColumn;      // Tambahan: mengingat kolom saat navigasi vertikal
extern char text[MAX_LINES][MAX_LENGTH];

// Core
void gotoxy(int x, int y);
void render();

// Editor logic
void moveCursor(int key);
void insertChar(char ch);
void backspace();
void enterKey();
void deleteChar();

#endif