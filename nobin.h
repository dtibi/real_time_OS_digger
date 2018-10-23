#ifndef NOBIN_FILE
#define NOBIN_FILE
#include "digger.h"
typedef struct nobbin {
	Digger *digger;
	int x;
	int y;
	char direction; //u= up, d= down, l= left, r= right
} Nobbin;

void move();
char find_direction_to_digger(Nobbin n);
Nobbin create_nobbin(Digger *d);

#endif
