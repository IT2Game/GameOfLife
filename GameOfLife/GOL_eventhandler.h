#pragma once

#include<SFML\Graphics.hpp>
#include<memory>

namespace gol {
	class EventHandler {
	private:
		sf::RenderWindow* window;

	public:
		EventHandler(sf::RenderWindow* window);

		void handleEvent(sf::Event &event);
	};
}