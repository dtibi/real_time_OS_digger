#ifndef MYINTS_FILE
#define MYINTS_FILE

#define LEFT_ARROW 75
#define RIGHT_ARROW 77
#define DOWN_ARROW 80
#define UP_ARROW 72

extern volatile unsigned char scan;
extern void interrupt (*Int9Save) (void);
extern void interrupt (*Int8Save) (void);
void interrupt MyISR9(void);
void interrupt MyISR8(void);

#endif