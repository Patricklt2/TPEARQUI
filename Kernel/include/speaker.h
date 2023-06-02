#ifndef _SPEAKER_H_
#define _SPEAKER_H_

#include <stdint.h>

#define C1 33
#define D1 37
#define E1 41
#define F1 44
#define G1 49
#define A1 55
#define B1 62

#define C2 65
#define D2 73
#define E2 82
#define F2 87
#define G2 98
#define A2 110
#define B2 123

#define C3 131
#define D3 147
#define E3 165
#define F3 175
#define G3 196
#define A3 220
#define B3 247

#define C4 262
#define D4 294
#define E4 330
#define F4 349
#define G4 392
#define A4 440
#define B4 494

#define C5 523
#define A5 880
#define B5 988
#define D5 587
#define E5 659
#define F5 698
#define G5 784


void playSound(uint32_t freq);
void stopSound();

// Make a simple beep, for a 50ms
void beep(void);
void marioTheme(void);
void imperialMarch(void);
void tetris(void);
void ffVFanfare(void);
void songOfStorms(void);
void userLandStartTheme(void);

#endif