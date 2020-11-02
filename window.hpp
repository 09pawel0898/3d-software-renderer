#ifndef WINDOW_H
#define WINDOW_H


#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

class Window : public sf::RenderWindow
{

public:
	Window() : sf::RenderWindow() {}
	void render();
};

#endif