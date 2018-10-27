#ifndef DIGGER_FILE
#define DIGGER_FILE

typedef struct digger {
	int x;
	int y;
	int direction; //LEFT_ARROW || RIGHT_ARROW || DOWN_ARROW || UP_ARROW
	} Digger;

void move_digger(Digger *player,int direction);
void run_digger(Digger *player);
void fire(Digger* player);
Digger create_digger();
int* directionChanged (int prevDirection, int nextDirection);
void goldFalling(int x, int y);

#endif
