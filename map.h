#ifndef MAP_FILE
#define MAP_FILE

typedef struct map {
	int id;
	int size;
	int **screen;
	} Map;

void clean_screen(int screen_color){
	asm{
	MOV 		AH,0          // Select function = 'Set mode'
	MOV 		AL,3          // 40 by 25 color image
	INT 		10h           // Adapter initialized. Page 0 displayed	

	MOV             AX,0B800h
	MOV             ES,AX
	MOV             DI,0
	MOV             AL,' '
	MOV             AH,BYTE PTR screen_color
	MOV             CX,50
	CLD
	REP             STOSW

	}
}


Map create_map(int id, int size){
	Map m;
	m.id = id;
	m.size = size;
	clean_screen(110);
	return m;
}


#endif
