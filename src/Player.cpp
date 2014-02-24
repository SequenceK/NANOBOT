#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include "Player.hpp"

Player::Player(float x, float y, int w, int h, sf::Texture& t) : sf::Sprite(t), x(x), y(y){
	//this->x = x;
	//this->y = y;
	
	this->width = w;
	this->height = h;
	this->setPosition(this->x, this->y);
	this->setScale(width/(getGlobalBounds().width),height/(getGlobalBounds().height));
	//this->texture = t;
}

Player::Player(float x, float y, sf::Texture& t) : sf::Sprite(t), x(x), y(y){

	this->width = getGlobalBounds().width;
	this->height = getGlobalBounds().height;
	this->setPosition(this->x, this->y);

}


void Player::init(){
	//this->setTexture(texture);
	this->setPosition(this->x, this->y);
	this->setScale(this->width/(this->getGlobalBounds().width),this->height/(this->getGlobalBounds().height));
}