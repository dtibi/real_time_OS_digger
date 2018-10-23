#include <stdio.h>
#include <dos.h>
#include "digger.h"
#include "map.h"
#include "myints.h"

void create_digger(Digger *player)
{
	(*player).x = 40;
	(*player).y = 23;
	(*player).direction = 'r';
}

