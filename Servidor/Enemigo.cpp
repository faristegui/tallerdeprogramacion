#include "Enemigo.h"


Enemigo::Enemigo(std::string unIdSprite)
{
	IDSprite = unIdSprite;
	x = 740;
	y = 425;
}
int Enemigo::getX()
{
	return x;
}

void Enemigo::mover()
{
	if(x !=0)
	{
		x-= 20;
	}
	else
	{
		x = 740;
	}
}

int Enemigo::getY()
{
	return y;
}
Enemigo::~Enemigo(void)
{
}
