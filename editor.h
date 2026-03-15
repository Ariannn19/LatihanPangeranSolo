#ifndef EDITOR_H
#define EDITOR_H

#define MAX_LINES 100
#define MAX_COLS 200

extern char text[MAX_LINES][MAX_COLS];
extern int lineCount;

void displayText();
void inputtext();

#endif