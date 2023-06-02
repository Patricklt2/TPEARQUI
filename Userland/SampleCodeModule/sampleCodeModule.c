/* sampleCodeModule.c */

#include <stdio.h>
#include "terminal.h"
#include "songs.h"
int main() {

    printf("Welcome to Userland!\n");
    // userLandStartTheme();
    printf("Choose a mode:\nPress 1 for Pong.\nPress 2 for Terminal.\nPress 3 for exit.\n");
    unsigned char keepGoing = TRUE;

    printf("%x", timestamp());
    while(keepGoing){
        char c = getChar();
        switch(c){
            case '1':
                break;
            case '2':
                terminalStart();
                printf("Choose a mode:\nPress 1 for Pong.\nPress 2 for Terminal.\nPress 3 for exit.\n");
                break;
            case '3':
                keepGoing = FALSE;
                break;
            default:
                break;
        }
    }

	return 0;
}