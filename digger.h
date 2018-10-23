#ifndef DIGGER_FILE
#define DIGGER_FILE

typedef struct digger {
	int x;
	int y;
	char direction; //u= up, d= down, l= left, r= right
	int speed;
	} Digger;

void move(Digger* player);
void fire(Digger* player);
Digger create_digger();

#endif
