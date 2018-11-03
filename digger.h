#ifndef DIGGER_FILE
#define DIGGER_FILE

typedef struct digger {
	int x;
	int y;
	int direction; //LEFT_ARROW || RIGHT_ARROW || DOWN_ARROW || UP_ARROW
	char is_alive;
	char lives;
	int score;
	int last_time_shot;
	} Digger;

void move_digger(Digger *player, int direction);
void fire(Digger* player);
Digger create_digger();
void digger_death_flow();
void restart_digger(Digger* player);

#endif
