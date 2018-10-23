#ifndef DIGGER_FILE
#define DIGGER_FILE

typedef struct digger {
	int x;
	int y;
	int direction; //LEFT_ARROW || RIGHT_ARROW || DOWN_ARROW || UP_ARROW
	} Digger;

void move(Digger* player);
void fire(Digger* player);
Digger create_digger();
void move_digger(Digger *player);
int* directionChanged (int prevDirection, int nextDirection);

#endif
