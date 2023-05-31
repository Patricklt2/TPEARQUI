#include "include/terminalHandler.h"
#include "include/keyboard.h"
#include "include/strings.h"
#include "include/time.h"
#include "include/lib.h"
#include <stdio.h>
#include <stdarg.h>
#include <screen.h>
#include <speaker.h>

#define SIZEOFARR(arr) (sizeof(arr)/sizeof(arr[0]) )

extern void opCode();

void showTime();
void exit();
void showDate();
void help(char * token);
void divZero();
void opCode();

typedef struct {
    char name[20];
    char description[100];
    void (*function)(void);
} commandT;

const commandT commands[] = {{"time","Shows the current time in GMT-3",showTime},
                             {"clear","clears screen and resets position",clearScreen},
                             {"date","Displays current date.",showDate},
                             {"exit","Exits the bash",exit},
                             {"bell","Outputs a Beep", beep},
                             {"66","Displays imperial march for starwars fans", imperialMarch},
                             {"mario","Displays mario bros theme song",marioTheme},
                             {"tetris","Displays tetris song",tetris},
                             {"storm","Displays song of storms zelda",songOfStorms},
                             {"div0","Shows how div 0 exception works",divZero},
                             {"opCode","Shows how opCode exception works",opCode}};

static unsigned char keepGoing = TRUE;

int terminalStart(){
    char* ptr = (char*)myMalloc(sizeof(char)*79);

    while(keepGoing){
        setTerminal();
        printf("$ ");

        scanf("%s",ptr);
        char *token = strtok(ptr," ");      //creo token con cmdline (modificable)
        // Process the command and execute actions accordingly
        printf("\n");

        if(!strcmp(token,"help")){
            help(token);
        } else {
            int i = 0;
            char * aux = strtok(NULL," ");

            if(aux != NULL){
                printf("This function does not accept arguments.\n");
            } else {
                while (i < SIZEOFARR(commands)) {
                    if (!strcmp(token, commands[i].name)) {
                        commands[i].function();
                        break;
                    }
                    i++;
                }
                if (i == SIZEOFARR(commands)) {
                    printf("Comando no encontrado.\n");
                }
            }
        }

        printf("\n");

    }

    myFree(ptr);
    return 0;
}

void help(char * token){
    char *aux = NULL;

    token = strtok(NULL," ");

    aux = strtok(NULL," ");

    if(aux != NULL){
        printf("help requires only one argument.\n");
        return ;
    } else if(token == NULL){
        printf("Current functions are:\n");
        for(int i = 0; i < SIZEOFARR(commands); i++){
            printf("%s\n", commands[i].name);
        }
        return;
    }else if(!strcmp(token,"help")){
        printf("Provides a list of functions or\nif an argument is passed\na brief description of the function passed as arg.\n");
        return;
    }

        for( int i = 0; i < SIZEOFARR(commands); i++){
            if(!strcmp(token, commands[i].name)){
                printf("%s\n",commands[i].description);
               return;
            }
        }
            printf("Desired function does not exist.\n");
}

void showTime(){
    char time[50];
    timeToStr(time);
    printf("%s\n",time);
    return;
}

void showDate(){
    char date[50];
    dateToStr(date);
    printf("%s\n",date);
    return;
}

void exit(){
    keepGoing = FALSE;
}

int divide(int x, int y){
    return x/y;
}

void divZero(){
    divide(0,0);
}

/*
 * TOADD
 * showRegisters(); ---> Con interrupcion
 * Rtc();   --> Con interrupcion
 * self cleaning y que escriba en la ultima linea
 * generateXException(); --> genere div 0 y haga un seguimiento ?) y verifica su funcionamiento
 *
 */