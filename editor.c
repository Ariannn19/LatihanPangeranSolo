#include <stdio.h>
#include <string.h>
#include "editor.h"

char text[MAX_LINES][MAX_COLS];
int lineCount = 0;

void displayText() {
    for(int i = 0; i < lineCount; i++) {
        printf("%d: %s\n", i+1, text[i]);
    }
}

void inputtext(){

    printf("Masukkan teks (ketik 'END' untuk selesai):\n");

    lineCount = 0;


    while(lineCount < MAX_LINES) {

        fgets(text[lineCount], MAX_COLS, stdin);
        text[lineCount][strcspn(text[lineCount], "\n")] = '\0';

     
        if(strcmp(text[lineCount], "END") == 0) {
            break;
        }

        lineCount++;
        printf("DEBUG lineCount sekarang: %d\n", lineCount);
    }
}