#ifndef DIGGER_FILE
#define DIGGER_FILE

typedef struct digger {
	int x;
	int y;
	int direction; //LEFT_ARROW || RIGHT_ARROW || DOWN_ARROW || UP_ARROW
	char is_alive;
	char lives;
	int score;
	long last_time_shot;
	long last_time_cherry;
	} Digger;

void move_digger(int direction);
void fire(Digger* player);
Digger create_digger();
void digger_death_flow();
void restart_digger(Digger* player);

#endif
