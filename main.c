#include <stdio.h>
#include "editor.h"
#include "find.h"

int main() {

    int choice;

    do {

        printf("\n=== TEXT EDITOR ===\n");
        printf("1. Display Text\n");
        printf("2. Input Text\n");
        printf("3. Find word\n");
        printf("4. Exit\n");

        printf("Choice: ");
        scanf("%d",&choice);
        getchar();

        switch(choice) {

            case 1:
                displayText();
                break;
            case 2:
                inputtext();
                break;
            case 3:
                findword(text,lineCount);
        }

    } while(choice != 4);

    return 0;
}