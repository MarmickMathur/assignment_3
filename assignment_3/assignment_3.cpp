#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>
#include "gameEngine.h"


int main()
{
	gameEngine game;
	game.init("src/config.txt");
	game.run();
}

