#ifndef PLAYER_HPP
#define PLAYER_HPP
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

class Player : public sf::Sprite{
	private:
		float x, y;
		int width, height;
		sf::Texture texture;
	public:
		Player(float x,float y,int w,int h, sf::Texture& t);
		Player(float x,float y, sf::Texture& t);
		void init();

};

#endif