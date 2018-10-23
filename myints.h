#ifndef MYINTS_FILE
#define MYINTS_FILE

extern volatile unsigned char scan;
extern void interrupt (*Int9Save) (void);
extern void interrupt (*Int8Save) (void);
void interrupt MyISR9(void);
void interrupt MyISR8(void);

#endif