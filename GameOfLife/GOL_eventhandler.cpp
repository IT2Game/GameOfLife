#include "GOL_eventhandler.h"

gol::EventHandler::EventHandler(sf::RenderWindow *window)
{
	this->window = window;
}

void gol::EventHandler::handleEvent(sf::Event & event)
{
	if (event.type == sf::Event::Closed)
		window->close();
}
