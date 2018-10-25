#ifndef NOBIN_FILE
#define NOBIN_FILE
#include "digger.h"

typedef struct nobbin {
	Digger *digger;
	int x;
	int y;
	char direction; 
	char is_alive;
	char is_hobbin;
} Nobbin;

void move_nobbins();
char find_direction_to_digger(Nobbin n);
Nobbin create_nobbin(Digger *d);

#endif
