#include <naiveConsole.h>
#include "include/time.h"

extern uint32_t dVideo(uint8_t * puntero, char * str);
extern int getCurrentTime(void);
static uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base);

static char buffer[64] = { '0' };
static uint8_t * const video = (uint8_t*)0xB8000;
static uint8_t * currentVideo = (uint8_t*)0xB8000;
static const uint32_t width = 80;
static const uint32_t height = 25 ;

void ncPrintChar(char character)
{
	*currentVideo = character;
	currentVideo += 2;
}

void ncPrint(const char * string)
{
    int i;

    for (i = 0; string[i] != 0; i++)
        ncPrintChar(string[i]);
}
void ncNewline()
{
	do
	{
		ncPrintChar(' ');
	}
	while((uint64_t)(currentVideo - video) % (width * 2) != 0);
}

void ncPrintDec(uint64_t value)
{
	ncPrintBase(value, 10);
}

void ncPrintHex(uint64_t value)
{
	ncPrintBase(value, 16);
}

void ncPrintBin(uint64_t value)
{
	ncPrintBase(value, 2);
}

void ncPrintBase(uint64_t value, uint32_t base)
{
    uintToBase(value, buffer, base);
    ncPrint(buffer);
}

void ncClear()
{
	int i;

	for (i = 0; i < height * width; i++)
		video[i * 2] = ' ';
	currentVideo = video;
}

static uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base)
{
	char *p = buffer;
	char *p1, *p2;
	uint32_t digits = 0;

	//Calculate characters for each digit
	do
	{
		uint32_t remainder = value % base;
		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
		digits++;
	}
	while (value /= base);

	// Terminate string in buffer.
	*p = 0;

	//Reverse string in buffer.
	p1 = buffer;
	p2 = p - 1;
	while (p1 < p2)
	{
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}

	return digits;
}


//Seccion putChar - puts - printf
int putChar(int c){
        switch(c){
            case '\n':
                handleEnter();
                return 0;
            case '\b':
                handleBackSpace();
                return 0;
            case '\t':
                handleTab();
                return 0;
            case '\0':
                return 0;
            case 27:            //caso del esc
                return 0;
            default:
                ncPrintChar(c);
                return 0;
        }
}

//Putchar verison string
int puts(const char * string){
    int i;
    int flag = 0;
    for (i = 0; string[i] != 0; i++) {
        flag = putChar(string[i]);

        if(flag != 0){
            return flag;
        }
    }

    return flag;
}

//Printf
int printf(const char * format, ...){
    va_list args;
    va_start(args, format);
    while (*format != '\0') {
        if (*format == '%') {
            format++;
            switch (*format++) {
                case 'd': {
                    int int_arg = va_arg(args, int);
                    ncPrintDec(int_arg);
                    break;
                }
                case 's': {
                    char* s_arg = va_arg(args, char*);
                    puts(s_arg);
                    break;
                }
                case 'c': {
                    char c_arg = va_arg(args, int);
                    putChar(c_arg);
                    break;
                }
                case 'x': {
                    int hex_arg = va_arg(args, int);
                    ncPrintHex(hex_arg);
                    break;
                }
                default:
                    break;
            }
        } else {
            putChar(*format++);
        }
    }
    return 0;
    va_end(args);
}
//section video driver
void writeMsg(char * str){
   currentVideo = dVideo(currentVideo,str);
    ncNewline();
}

// Sectio kb
void handleBackSpace(){
    if(currentVideo > video) {
        currentVideo -= 2;      //voy para atras en el kernel
        *currentVideo = '\0';   //cambio el actual a un espacio
    }
}

void handleTab(){
    for(int i  = 0; i < 3; i++){
        *currentVideo = ' ';
        currentVideo += 2;
    }
}

void handleEnter(){
    ncNewline();
}





