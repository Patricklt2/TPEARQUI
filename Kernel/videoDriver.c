
#include <videoDriver.h>



// realiza el chequeo de fuera de bordes retorna 1 si esta fuera, 0 si no.
int failBordersCheck( uint32_t x, uint32_t y );

struct vbe_mode_info_structure {
	uint16_t attributes;		// deprecated, only bit 7 should be of interest to you, and it indicates the mode supports a linear frame buffer.
	uint8_t window_a;			// deprecated
	uint8_t window_b;			// deprecated
	uint16_t granularity;		// deprecated; used while calculating bank numbers
	uint16_t window_size;
	uint16_t segment_a;
	uint16_t segment_b;
	uint32_t win_func_ptr;		// deprecated; used to switch banks from protected mode without returning to real mode
	uint16_t pitch;			// number of bytes per horizontal line
	uint16_t width;			// width in pixels
	uint16_t height;			// height in pixels
	uint8_t w_char;			// unused...
	uint8_t y_char;			// ...
	uint8_t planes;
	uint8_t bpp;			// bits per pixel in this mode
	uint8_t banks;			// deprecated; total number of banks in this mode
	uint8_t memory_model;
	uint8_t bank_size;		// deprecated; size of a bank, almost always 64 KB but may be 16 KB...
	uint8_t image_pages;
	uint8_t reserved0;
 
	uint8_t red_mask;
	uint8_t red_position;
	uint8_t green_mask;
	uint8_t green_position;
	uint8_t blue_mask;
	uint8_t blue_position;
	uint8_t reserved_mask;
	uint8_t reserved_position;
	uint8_t direct_color_attributes;

	uint32_t framebuffer;		// physical address of the linear frame buffer; write here to draw to the screen
	uint32_t off_screen_mem_off;
	uint16_t off_screen_mem_size;	// size of memory in the framebuffer but not being displayed on the screen
	uint8_t reserved1[206];
} __attribute__ ((packed));

typedef struct vbe_mode_info_structure * VBEInfoPtr;

VBEInfoPtr VBE_mode_info = (VBEInfoPtr) 0x0000000000005C00;

uint16_t horizontalPixelCount;
uint16_t verticalPixelCount;
uint64_t numberOfColorBytes;


void vd_Initialize(){
	horizontalPixelCount = VBE_mode_info->width;
	verticalPixelCount = VBE_mode_info->height;
	numberOfColorBytes = VBE_mode_info->bpp/8;
}

/*
void putPixel(uint8_t r, uint8_t g, uint8_t b, uint32_t x, uint32_t y) {
	uint8_t * videoPtr = VBE_mode_info->framebuffer;
	int offset = y * VBE_mode_info->pitch + x * (VBE_mode_info->bpp / 8);
	videoPtr[offset] = b;
	videoPtr[offset+1] = g;
	videoPtr[offset+2] = r;
}
*/
void putPixel( uint32_t x, uint32_t y, uint32_t hexColor) {
	uint8_t * screen = (uint8_t *) ((uint64_t) (VBE_mode_info->framebuffer));
    uint32_t offset = VBE_mode_info->pitch*y + x*numberOfColorBytes;
	screen[offset] = hexColor & 0xFF;
	screen[offset+1] = (hexColor >> 8) & 0xFF;
	screen[offset+2] = (hexColor >> 16) &  0xFF;
}


// necesario para cuando se haga la subida de pantalla
// pasa el pixel y tod su color de un sector de pantalla a otro
void copyPixel( uint32_t xfrom, uint32_t yfrom, uint32_t xto, uint32_t yto ){

	uint8_t * screen = (uint8_t *) ((uint64_t) (VBE_mode_info->framebuffer));
	// creo el offset de form
	uint32_t offsetFrom = VBE_mode_info->pitch*yfrom + xfrom*numberOfColorBytes;
	// creo el offset de to
	uint32_t offsetTo = VBE_mode_info->pitch*yto + xto*numberOfColorBytes;
	// paso uno al otro
	screen[offsetTo] = screen[offsetFrom];
	screen[offsetTo+1] = screen[offsetFrom+1];
	screen[offsetTo+2] = screen[offsetFrom+2];
}

// chequear esta funcion pues el string se puede cortar en pantalla
void draw_string(uint32_t x, uint32_t y, char* input,uint32_t fontColor,
				 uint32_t backgroundColor) {

    while(*input) {
        draw_char(x,y,input[0],fontColor, backgroundColor);
        x += CHAR_WIDTH;
		if ( failBordersCheck(x, y)){
			y+=CHAR_HEIGHT;
			x = 0;
		}
        input++;
    }
}


void draw_char( uint32_t x, uint32_t y, char character, uint32_t fontColor,
				 uint32_t backgroundColor ){

	if ( failBordersCheck(x, y)){
		return;
	}

	uint32_t auxx = x, auxy = y;
	char pixIsPresent = 0;

	uint8_t* font_char = charBitmap(character);

	for(int i = 0; i < CHAR_HEIGHT; i++){
		for(int j = 0; j < CHAR_WIDTH; j++){
			//veo si tengo que imprimir el pixel o no, +2 para que imprima bien con el correcto decalaje de bits
			pixIsPresent = (1 << (CHAR_WIDTH + 2 - j)) & font_char[i];
			if ( pixIsPresent )
				putPixel( auxx, auxy, fontColor );
			else
				putPixel( auxx, auxy, backgroundColor );
				auxx++;
		}
		auxx = x;
		auxy++;
	}
}


int failBordersCheck( uint32_t x, uint32_t y ){
	//chequeo los limites (es - o / comparando con la cantidad de espacio del char en los bordes)
	if ( x < 0 || x > horizontalPixelCount - CHAR_WIDTH || y < 0
				|| y > verticalPixelCount - CHAR_HEIGHT){
		return 1;
	}
	return 0;
}

uint16_t getHorizontalPixelCount(){
	return horizontalPixelCount;
}
uint16_t getVerticalPixelCount(){
	return verticalPixelCount;
}