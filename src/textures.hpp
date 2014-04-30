#ifndef TEXTURES_HPP
#define TEXTURES_HPP
#include <SFML/Graphics.hpp>

namespace Tex {
	sf::Texture box;
	sf::Texture tEnd;
	sf::Texture tooth;
	sf::Texture player;
	sf::Texture fBG;
	sf::Texture bBG;
	sf::Texture bFG;
};

void loadTextures()
{
	Tex::box.loadFromFile("../data/t.png");
	Tex::tEnd.loadFromFile("../data/tEnd.png");
	Tex::tooth.loadFromFile("../data/tooth.png");
	Tex::player.loadFromFile("../data/player.png");
	Tex::fBG.loadFromFile("../data/face.png");
	Tex::bBG.loadFromFile("../data/body.png");
	Tex::bFG.loadFromFile("../data/bodyFG.png");
};


#endif