#include<SFML\Graphics.hpp>
#include<thread>
#include<iostream>
#include<memory>

#include"GOL_eventhandler.h"
#include"GOL_Process.h"

std::string applicationName = "Game of Life";
sf::VideoMode startVideoMode(1280, 720);

int main() {
	sf::RenderWindow *window = new sf::RenderWindow(startVideoMode, applicationName);
	window->setFramerateLimit(60);
	
	gol::Process proc;
	gol::EventHandler eventHandler(window);

	while (window->isOpen()) {
		sf::Event event;
		
		while (window->pollEvent(event))
			eventHandler.handleEvent(event);

		window->clear();
		proc.update();
		proc.draw(window);
		window->display();
	}
}