

#include <screen.h>
#include <string.h>
#include <time.h>
// tamaño de pantalla
// 1024 x 768

void bordersCheck();

uint64_t screenWidth;
uint64_t screenHeight;
uint32_t current_cursor_pos_x = 0;
uint32_t current_cursor_pos_y = 0;

void screen_Initialize(){
    vd_Initialize();
    screenHeight = getVerticalPixelCount();
    screenWidth = getHorizontalPixelCount();
}

// setea el modo terminal para que imprima en la ultima linea
void setTerminalPrintingMode(){
    current_cursor_pos_x = 0;
    current_cursor_pos_y = screenHeight - CHAR_HEIGHT;
}


// tabla de caracteres especiales
// tabla de relaciones con letras

void printStrScreenFrmt( char * str, uint32_t font_color, uint32_t background_Color ){
    while(*str != '\0'){
        putCharScreenFrmt(*str++,font_color,background_Color);
    }
}

void printErrorStr( char* str ){
    printStrScreenFrmt(str,RED, COLOR_BACKGROUND_DEFAULT);
}



void putCharScreen( char character ){
    putCharScreenFrmt(character,COLOR_LETTER_DEFAULT, COLOR_BACKGROUND_DEFAULT);
}

void putCharScreenFrmt( char character, uint32_t font_color, uint32_t background_Color ){
    bordersCheck();
    // imprimo el caracter en la pantalla
    draw_char( current_cursor_pos_x, current_cursor_pos_y, character,
     font_color, background_Color );
    current_cursor_pos_x +=CHAR_WIDTH;
}
// retrocede 1 char limpiando su contenido
void backspace(){
    if ( current_cursor_pos_x == 0 ){
        current_cursor_pos_y-=CHAR_HEIGHT;
    }
    current_cursor_pos_x -= CHAR_WIDTH;
    putCharScreen(' ');
    current_cursor_pos_x -= CHAR_WIDTH;
}
// imprime 2 char ' ' 
void tab(){
    putCharScreen(' ');
    putCharScreen(' ');
    putCharScreen(' ');
    putCharScreen(' ');
}
// pasa a la linea de abajo
void enter(){
    if ( current_cursor_pos_y <= screenHeight - 2*CHAR_HEIGHT ){
        current_cursor_pos_y += CHAR_HEIGHT;
    }else{
        scrollScreenUp();
    }
    current_cursor_pos_x = 0; 
}

void scrollScreenUp(){
    // cada pixel lo copio en la de arriba TODO optimizar
    for ( int i= 0; i < screenHeight - CHAR_HEIGHT ; i++ ){ // y (altura)
        for( int j=0; j < screenWidth ; j++ ){ // x (ancho)
            copyPixel( j,i+CHAR_HEIGHT,j,i ); // copio al de arriba
        }
    }
    // imprimo en la ult linea espacios
    
    for ( int i= 0; i< screenWidth; i+=CHAR_WIDTH ){
        draw_char( i, screenHeight - CHAR_HEIGHT, ' ',
         COLOR_LETTER_DEFAULT, COLOR_BACKGROUND_DEFAULT );
    }
    
    current_cursor_pos_x = 0;
    current_cursor_pos_y = screenHeight - CHAR_HEIGHT;
    
}

void clearScreen(){

    //imprimo en backgroundColor en toda la pantalla
    for ( int i= 0; i < screenHeight +6; i++ ){
        for( int j=0; j < screenWidth ; j++ ){
            putPixel( j,i,COLOR_BACKGROUND_DEFAULT );
        }
    }
    // restauro el cursor al inicio
    current_cursor_pos_x = 0;
    current_cursor_pos_y = 0;
}

void bordersCheck(){
    if ( current_cursor_pos_x <= screenWidth - CHAR_WIDTH )
        return;

    current_cursor_pos_x = 0;
    if( current_cursor_pos_y <= screenHeight - 2*CHAR_HEIGHT){
            current_cursor_pos_y += CHAR_HEIGHT;
    }else{
        scrollScreenUp();
    }
}
  
