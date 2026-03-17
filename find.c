#include <stdio.h>
#include <string.h>
#include "editor.h"
#include "find.h"

void findword(char text[][100], int lineCount) {

    char keyword[50];
    int found = 0;

    printf("Masukkan kata yang ingin dicari: ");

    fgets(keyword, sizeof(keyword), stdin);
    keyword[strcspn(keyword, "\n")] = '\0';

    for(int i = 0; i < lineCount; i++) {

        // DEBUG
       // printf("DEBUG text[%d]: [%s]\n", i, text[i]);
        //printf("DEBUG keyword: [%s]\n", keyword);

        if(strstr(text[i], keyword) != NULL) {
            printf("Ditemukan di baris %d: %s\n", i + 1, text[i]);
            found = 1;
        }
    }

    if(!found) {
        printf("Kata tidak ditemukan.\n");
    }
}