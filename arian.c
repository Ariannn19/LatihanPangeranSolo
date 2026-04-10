#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include "arian.h"

int cursorX = 0;
int cursorY = 0;
int lines = 1;
char text[MAX_LINES][MAX_LENGTH] = {0};

void gotoxy(int x, int y) {
    COORD c = {x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void render() {
    system("cls");

    for (int i = 0; i < lines; i++) {
        gotoxy(0, i);
        printf("%s", text[i]);
    }

    if (cursorY >= lines) cursorY = lines - 1;
    if (cursorY < 0) cursorY = 0;

    int len = strlen(text[cursorY]);
    if (cursorX > len) cursorX = len;
    if (cursorX < 0) cursorX = 0;

    gotoxy(cursorX, cursorY);
}

void moveCursor(int key) {
    int len = strlen(text[cursorY]);
    int newLen;

    if (key == 72) { // UP
        if (cursorY > 0) {
            cursorY--;
            newLen = strlen(text[cursorY]);
            if (cursorX > newLen) cursorX = newLen;
        }
    }
    else if (key == 80) { // DOWN
        if (cursorY < lines - 1) {
            cursorY++;
            newLen = strlen(text[cursorY]);
            if (cursorX > newLen) cursorX = newLen;
        }
    }
    else if (key == 75) { // LEFT
        if (cursorX > 0) {
            cursorX--;
        }
        else if (cursorY > 0) {
            cursorY--;
            cursorX = strlen(text[cursorY]);
        }
    }
    else if (key == 77) { // RIGHT
        if (cursorX < len) {
            cursorX++;
        }
        else if (cursorY < lines - 1) {
            cursorY++;
            cursorX = 0;
        }
    }
}

void insertChar(char ch) {
    int len = strlen(text[cursorY]);
    if (cursorX > len) cursorX = len;

    // Kasus 1: baris belum penuh → sisipkan normal
    if (len < MAX_LENGTH - 1) {
        for (int i = len; i >= cursorX; i--) {
            text[cursorY][i + 1] = text[cursorY][i];
        }
        text[cursorY][cursorX] = ch;
        cursorX++;
        return;
    }

    // Kasus 2: Baris penuh → cascading shift
    char currentChar = ch;
    int currentY = cursorY;
    int insertX = cursorX;

    while (currentY < MAX_LINES) {
        // Buat baris baru jika perlu
        if (currentY >= lines) {
            lines++;
            text[currentY][0] = '\0';
        }

        int currentLen = strlen(text[currentY]);

        if (currentLen < MAX_LENGTH - 1) {
            // Ada ruang, sisipkan currentChar
            for (int i = currentLen; i >= insertX; i--) {
                text[currentY][i + 1] = text[currentY][i];
            }
            text[currentY][insertX] = currentChar;
            break;
        } else {
            // Baris penuh, karakter paling kanan jatuh
            char overflowChar;
            if (insertX == MAX_LENGTH - 1) {
                overflowChar = currentChar;
            } else {
                overflowChar = text[currentY][MAX_LENGTH - 2];
                // Geser ke kanan dari insertX sampai sebelum karakter terakhir
                for (int i = MAX_LENGTH - 3; i >= insertX; i--) {
                    text[currentY][i + 1] = text[currentY][i];
                }
                text[currentY][insertX] = currentChar;
                // Null terminator tetap di MAX_LENGTH-1, tidak berubah
            }
            currentY++;
            currentChar = overflowChar;
            insertX = 0;
        }
    }

    // Update kursor
    cursorX++;
    if (cursorX >= MAX_LENGTH) {
        cursorX = 1;
        if (cursorY < lines - 1) {
            cursorY++;
        }
    }
}

void backspace() {
    if (cursorX > 0) {
        cursorX--;
        deleteChar();
    }
    else if (cursorY > 0) {
        cursorX = strlen(text[cursorY - 1]);
        cursorY--;
        deleteChar();
    }
}

void deleteChar() {
    int len = strlen(text[cursorY]);

    if (cursorX < len) {
        // Hapus karakter di posisi kursor: geser ke kiri
        for (int i = cursorX; i < len; i++) {
            text[cursorY][i] = text[cursorY][i + 1];
        }
    }
    else if (cursorY < lines - 1) {
        // Tarik karakter dari baris berikutnya
        int currLen = len;
        int nextLen = strlen(text[cursorY + 1]);
        int spaceLeft = MAX_LENGTH - 1 - currLen;

        int copyCount = nextLen;
        if (copyCount > spaceLeft) copyCount = spaceLeft;

        // Salin karakter dari baris berikutnya ke akhir baris ini
        for (int i = 0; i < copyCount; i++) {
            text[cursorY][currLen + i] = text[cursorY + 1][i];
        }
        text[cursorY][currLen + copyCount] = '\0';

        if (nextLen > copyCount) {
            // Geser sisa karakter di baris berikutnya ke kiri
            for (int i = 0; i < nextLen - copyCount; i++) {
                text[cursorY + 1][i] = text[cursorY + 1][copyCount + i];
            }
            text[cursorY + 1][nextLen - copyCount] = '\0';
        } else {
            // Semua karakter baris berikutnya sudah dipindahkan, hapus baris
            for (int i = cursorY + 1; i < lines - 1; i++) {
                for (int j = 0; j <= MAX_LENGTH; j++) {
                    text[i][j] = text[i + 1][j];
                }
            }
            text[lines - 1][0] = '\0';
            lines--;
        }
    }
}

void enterKey() {
    if (lines >= MAX_LINES) return;

    char temp[MAX_LENGTH];
    strcpy(temp, &text[cursorY][cursorX]);
    text[cursorY][cursorX] = '\0';

    // Geser baris ke bawah (dari belakang)
    for (int i = lines; i > cursorY; i--) {
        for (int j = 0; j < MAX_LENGTH; j++) {
            text[i][j] = text[i - 1][j];
        }
    }

    strcpy(text[cursorY + 1], temp);
    lines++;
    cursorY++;
    cursorX = 0;
}