#include "terminal.h"

#define SIZEOFARR(arr) (sizeof(arr)/sizeof(arr[0]) )

extern void opCode();
extern void terminalSetter();
extern void setBackgroundColour(uint32_t colour);
extern void setForegroundColour(uint32_t colour);

void setBackground();
void setForeground();
void runCommand(char *);
void showTime();
void exit();
void showDate();
void help();
void divZero();
void beep();

typedef struct {
    char name[20];
    char description[150];
    void (*function)(void);
} commandT;

typedef struct{
    char color[15];
    int hex;
}colour;

const commandT commands[] = {
                             {"help", "Provides a command list.", help},
                             {"time","Shows the current time in GMT-3",showTime},
                             {"clear","clears screen and resets position",terminalSetter},
                             {"date","Displays current date.",showDate},
                             {"exit","Exits the bash",exit},
                             {"bell","Outputs a Beep", beep},
                             {"66","Displays imperial march for starwars fans", imperialMarch},
                             {"mario","Displays mario bros theme song",marioTheme},
                             {"tetris","Displays tetris song",tetris},
                             {"storm","Displays song of storms zelda",songOfStorms},
                             {"background","Changes background to hexColour: ",setBackground},
                             {"foreground","Changes foreground to hexColour: ",setForeground},
                             {"div0","Shows how div 0 exception works",divZero},
                             {"opCode","Shows how opCode exception works",opCode}
                            };

#define BUFFER_SIZE 50
#define INSTRUCTION_SIZE 240    

typedef struct{
    char command[INSTRUCTION_SIZE];
    uint8_t size;
} instructionT;

typedef struct{
    instructionT data[BUFFER_SIZE];
    uint64_t readWriteIndex, size;
} bufferT;

// Lo dejo en una zona de memoria q
static bufferT buffer = { 0 };

static unsigned char keepGoing = TRUE;

/**
 * changeCommand 
 * 
 * Cambia el commando actual impreso en la pantalla con lo que hay en el buffer. 
 */
int changeCommand(int insNumber, char * current){
    if(insNumber >= buffer.size)
        return -1;                                          // Nunca debería entrar acá

    for(int i = 0; current[i]; i++){ putChar('\b'); }       // Borro los caracteres anteriores.
    strcpy(buffer.data[insNumber].command, current);
    puts(buffer.data[insNumber].command);                                          // Imprimo los actuales

    return buffer.data[insNumber].size;
}

/**
 * Guarda el comando en el buffer. (Si el readWriteIndex es mayor o igual al BUFFER_SIZE, entonces readWriteIndex se pone en 0),
 * y cambio read.
 * 
 */
int saveCommand(char * current){
    buffer.size += buffer.size <= BUFFER_SIZE;                          // Cambio el tamaño del buffer

    int size =  buffer.data[buffer.readWriteIndex].size = strlen(current);
    strcpy(current, buffer.data[buffer.readWriteIndex].command);        // Guardo el commando.

    memset(current, 0, INSTRUCTION_SIZE);

    buffer.readWriteIndex = (buffer.readWriteIndex + 1) % BUFFER_SIZE;  // Cambio la posición en la que leo y guardo.
    return 0;
}

char * getInstruction(char * ptr){
    uint8_t c;
    int j = buffer.readWriteIndex - 1;
    int i = 0;
    while((c = getChar()) != '\n' && i < INSTRUCTION_SIZE){
        switch(c){
            case 0x80:      // Arrow UP     (Pone los comandos más viejos)
                if(buffer.size > 0){
                    i = changeCommand(j, ptr);
                    j = (j + buffer.size - 1) % buffer.size;
                }
                break;
            case 0x81:      // Arrow DOWN   (Pone los comandos más nuevos)
                if(buffer.size > 0){
                    i = changeCommand(j, ptr);
                    j = (j + 1) % buffer.size;
                }
                break;
            case '\b':
                if(i != 0){
                    putChar(c);
                    --i;
                }
                ptr[i] = 0;
                break;
            default:
                putChar(c);
                ptr[i++] = c;
                break;
        }
    }
    putChar(c);
    ptr[i] = 0;

    return ptr;
}


int terminalStart(){
    static char ptr[INSTRUCTION_SIZE] = { 0 };
    keepGoing = TRUE;
    // llamado de syscall para setear al modo terminal pasandole el 0 que indica este modo
    terminalSetter();
    while(keepGoing){
        printf("$ ");

        getInstruction(ptr);

        char *token = strtok(ptr, " ");     //creo token con cmdline (modificable)
                                            //Process the command and execute actions accordingly
        runCommand(token);

        printf("\n");

        saveCommand(ptr);
    }

    setBackgroundColour(BLACK);
    setForegroundColour(WHITE);
    return 0;
}

void runCommand(char * cmd){
    for(int i = 0; i < SIZEOFARR(commands); i++){
        if(!strcmp(cmd, commands[i].name)) {
            if(i == 0 || i == 10 || i == 11){
                commands[i].function();
                putChar('\n');
                return;
            }

            char * aux = strtok(NULL, " ");
            if(aux != NULL){
                printf("This function does not accept arguments.\n");
                return;
            }

            commands[i].function();
            putChar('\n');
            return;
        }

    }
        printf("Comando no encontrado.\n");


    return;
}

void help(/*char * token*/){
    char * token = strtok(NULL, " ");
    char * aux = strtok(NULL, " ");

    if(aux != NULL){
        printf("help requires only one argument.\n");
        return;
    }

    if(token == NULL){
        printf("Current functions are:\n");
        for(int i = 0; i < SIZEOFARR(commands); i++){
            printf("%s - %s\n", commands[i].name, commands[i].description);
        }
        return;
    }

    if(!strcmp(token,"help")){
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
    int hour,min,sec;
    getTime(&hour,&min,&sec);
    printf("%2d:%2d:%2d\n",hour,min,sec);
}

void showDate(){
    int year,month,day;
    getDate(&day,&month,&year);
    printf("%2d/%2d/%d\n",day,month,year);
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

void beep(){
    putChar('\a');
}


const colour colors[]= {{"black",BLACK},{"blue",BLUE},{"green",GREEN},
                        {"cyan",CYAN},{"red",RED},{"purple",PURPLE},
                        {"brown",BROWN},{"gray",GRAY},{"darkgray",DARK_GRAY},
                        {"lightblue",LIGHT_BLUE},{"lime",LIGHT_GREEN},{"lightcyan",LIGHT_CYAN},
                        {"lightred",LIGHT_RED},{"lightpurple",LIGHT_PURPLE},{"yellow",YELLOW},
                        {"white",WHITE}};

void nonExistentColor(){
    printf("Selected color is not available, please choose from the following list:\n");
    for(int j = 0; j < SIZEOFARR(colors)-1; j++){
        printf("%s, ",colors[j].color);
    }
    printf("%s \n\nExamples of usage: background cyan\n                   or\n                   foreground cyan",colors[SIZEOFARR(colors)-1].color);
    return;
}

void setBackground(){

    char * token = strtok(NULL, " ");
    char * aux = strtok(NULL, " ");

    if(aux != NULL){
        printf("Background requires only one argument.\n");
        return;
    }

    for(int i = 0; i < SIZEOFARR(colors); i++){
        if(!strcmp(colors[i].color,token)){
            setBackgroundColour(colors[i].hex);
            return;
        }
    }

    nonExistentColor();
    return;
}

void setForeground(){

    char * token = strtok(NULL, " ");
    char * aux = strtok(NULL, " ");

    if(aux != NULL){
        printf("Foreground requires only one argument.\n");
        return;
    }

    for(int i = 0; i < SIZEOFARR(colors); i++){
        if(!strcmp(colors[i].color,token)){
            setForegroundColour(colors[i].hex);
            return;
        }
    }

    nonExistentColor();
    return;
}
/*
 * TOADD
 * showRegisters(); ---> Con interrupcion
 * Rtc();   --> Con interrupcion
 * self cleaning y que escriba en la ultima linea
 * generateXException(); --> genere div 0 y haga un seguimiento ?) y verifica su funcionamiento
 *
 */