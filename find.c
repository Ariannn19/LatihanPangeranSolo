#include <stdio.h>
#include <string.h>
#include "editor.h"
#include "find.h"

void findword(char text[][100], int lineCount) {

    char keyword[50];
    int found = 0;

    printf("Masukkan kata yang ingin dicari: ");
    scanf("%49s", keyword);  // lebih aman (hindari overflow)
    getchar(); // bersihin newline dari buffer

    for(int i = 0; i < lineCount; i++) {

        // DEBUG (boleh dihapus nanti)
        // printf("DEBUG: [%s]\n", text[i]);

        if(strstr(text[i], keyword) != NULL) {

            printf("Ditemukan di baris %d: %s\n", i + 1, text[i]);
            found = 1;
        }
    }

    if(!found) {
        printf("Kata tidak ditemukan.\n");
    }
}