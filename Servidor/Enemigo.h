#pragma once
#include <string>
class Enemigo
{
public:
	Enemigo(std::string unIdSprite);
	int getX();
	int getY();
	void mover();
	~Enemigo(void);
private:
	std::string IDSprite;
	int x;
	int y;
};

