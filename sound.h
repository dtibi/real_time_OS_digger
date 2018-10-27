#ifndef SOUND_FILE
#define SOUND_FILE
#define ON (1)
#define OFF (0)

#define SHORT 0
#define LONG 1

#define C4 	262
#define C4S 277
#define D4 	294
#define D4S 311
#define E4 	330
#define F4 	349
#define F4S 370
#define G4 	392
#define G4S 415
#define A4 	440
#define A4S 466
#define B4 	494
#define C5 	523
#define C5S 554


void my_delay(int n,int type);
void change_speaker(int status);
void sound(int hertz);
void no_sound(void);

void beethoven(void);

void background_music(void);

#endif