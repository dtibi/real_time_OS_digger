#ifndef DIGGER_FILE
#define DIGGER_FILE

typedef struct digger {
	int x;
	int y;
	char direction; //u= up, d= down, l= left, r= right
	} Digger;

void move(Digger* player);
void fire(Digger* player);
void create_digger(Digger* player);
void move_digger(Digger *player);
#endif
