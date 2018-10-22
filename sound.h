#ifndef SOUND_FILE
#define SOUND_FILE

#define ON (1)
#define OFF (0)

void my_delay(int n);
void change_speaker(int status);
void sound(int hertz);
void no_sound(void);
void background_music(void);

#endif