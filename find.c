#include <stdio.h>
#include <string.h>
#include "editor.h"
#include "find.h"

void findword(char text[][100], int lineCount) {

    char keyword[50];

    printf("Masukkan kata yang ingin dicari: ");
    scanf("%s", keyword);
    getchar(); 

    int found = 0;

    for(int i = 0; i < lineCount; i++) {

        if(strstr(text[i], keyword) != NULL) {

            printf("Ditemukan di baris %d: %s", i+1, text[i]);
            found = 1;
        }
    }

    if(!found) {
        printf("Kata tidak ditemukan.\n");
    }
}