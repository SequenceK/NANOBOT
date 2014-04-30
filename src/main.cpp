#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <typeinfo>
#include "Player.hpp"
#include "game.hpp"

int main()
{
	G::Game game;
	game.newGame(true);
    return 0;
}