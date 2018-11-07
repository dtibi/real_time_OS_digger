#ifndef SOUND_FILE
#define SOUND_FILE
#define ON (1)
#define OFF (0)

//speed of sound delays
#define LONG_DELAY 300
#define SHORT_DELAY 50

#define BB4 247
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

void my_delay(int n, int type);
void change_speaker(int status);
void sound(int hertz);
void background_music();
void beethoven();
void play_death();
void enemy_killed_sound();
void fire_sound();
void diamond_taken_sound();
void cherry_taken_sound();
void sound_effects();

#endif
