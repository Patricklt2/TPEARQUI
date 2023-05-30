#include <stdio.h>

// Llama solo a getChar() y debe funcionar. 
char * getString(void){
    char c;
    char * str;
    char * buff = str;

    while((c = getChar()) != '\n'){
        switch(c){
            case '\b':
                if(str != buff)
                    *--str;
                *str = 0;
                break;
            default:
                *str++ = c;
                break;
        }
    }

    *str = 0;
    return buff;
}

/*
 *
 * El delimitador de cada scanf va a ser el '\n' indicando que se termino la linea
 * Los espacios vacios (white space seran ignorados siempre y cuando no sea un string)
 * Caso 'c' ==> lee un char
 * Caso 'd' ==> Lee un int de hasta 64 bit
 * Caso 's' ==> lee un str
 * Caso 'x' ==> lee un hexa de hasta-> lo pasa a dec
 *
 */
static int scanf_int(int64_t *);
static int scanf_str(char *);
static int scanf_hex(uint64_t *);

/*
 * Para que lo documente santi *-*
 */
int scanf(const char *format, ...) {
    va_list args;
    va_start(args,format);
    int flag_error = 0;

    while(*format != '\0' && !flag_error){
        if(*format == '%'){
            format++;
            switch(*format++){
                case 'd': {
                    int64_t *int_arg = va_arg(args,int64_t *);
                    if (scanf_int(int_arg) == -1) {
                        *int_arg = '\0';
                        flag_error = 1;
                        printf("Format error\n");
                    }
                    break;
                }
                case 'x': {
                    uint64_t *hex_arg = va_arg(args,uint64_t *);
                    if (scanf_hex(hex_arg) == -1) {
                        *hex_arg = '\0';
                        flag_error = 1;
                        printf("Format error\n");
                    }
                    break;
                }
                case 's': {
                    char *s_arg = va_arg(args,
                    char *);
                    scanf_str(s_arg);
                    break;
                }
                case 'c': {
                    char *c_arg = va_arg(args,char *);
                    *c_arg = getChar();
                    putChar('\n');
                    break;
                }
                default:
                    break;
            }
        } else {
            ++format;
        }
    }
    va_end(args);
    return 0;
}

//Func aux que procesa un String a Entero
static int scanf_int(int64_t * value_ptr){
    int64_t value = 0;
    char c;
    int sign = 1;

    while ((c = getChar()) != '\n') {
        if (c == '-') {
            sign *= -1;
            // if(sign != -1) {
            //     sign = -1;
            // } else {
            //     return -1;
            // }
        } else if (isdigit(c)) {
            value = value * 10 + (c - '0');
        } else if (c == '\b') {
            value = value/10;
        } else {
            putChar('\n');
            break;
        }
    }

    *value_ptr = sign * value;
    return 1;
}

//Func aux que copia el str a un puntero
static int scanf_str(char * s_arg){

    char * str = getString();
    strcpy(str, s_arg);

    return 0;
}

//Func aux que procesa un String (hex con fmt 0x o 0X) a entero
static int scanf_hex(uint64_t *hexToDec) {
    uint64_t result = 0;
    char * hexStr = getString();

    //Verifico que entre en formato de hexa 0x // 0X
    if(!(hexStr[0] == '0' && (hexStr[1] == 'x' || hexStr[1] == 'X'))){
        return -1;
    }

    for (int i = 2; hexStr[i] != '\0'; i++) {
        result *= 16;
        if (hexStr[i] >= '0' && hexStr[i] <= '9') {
            result += hexStr[i] - '0';
        } else if (hexStr[i] >= 'a' && hexStr[i] <= 'f') {
            result += hexStr[i] - 'a' + 10;
        } else if (hexStr[i] >= 'A' && hexStr[i] <= 'F') {
            result += hexStr[i] - 'A' + 10;
        } else {
            // no es un hexa dec valido
            return -1;
        }
    }
    *hexToDec = result;
    return 0;
}