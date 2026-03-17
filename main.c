#include <stdio.h>
#include "editor.h"

int main() {

    int choice;

    do {

        printf("\n=== TEXT EDITOR ===\n");
        printf("1. Display Text\n");
        printf("2. input Text\n");
        printf("3. Exit\n");

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
        }

    } while(choice != 3);

    return 0;
}