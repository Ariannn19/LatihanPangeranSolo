#include <stdio.h>
#include <string.h>
#include "editor.h"
#include "find.h"

void findword(char text[][MAX_COLS], int lineCount) {

    char keyword[50];
    int found = 0;

    printf("Masukkan kata yang ingin dicari: ");

    fgets(keyword, sizeof(keyword), stdin);
    keyword[strcspn(keyword, "\n")] = '\0';

    printf("DEBUG keyword: [%s]\n", keyword);
    printf("DEBUG lineCount: %d\n", lineCount);

    for(int i = 0; i < lineCount; i++) {

        printf("DEBUG text[%d]: [%s]\n", i, text[i]);

        if(strstr(text[i], keyword) != NULL) {
            printf("Ditemukan di baris %d: %s\n", i + 1, text[i]);
            found = 1;
        }
    }

    if(!found) {
        printf("Kata tidak ditemukan.\n");
    }
}