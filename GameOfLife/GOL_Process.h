#pragma once
#include<SFML\Graphics.hpp>

namespace gol {
	enum class Status {
			life, dead
	};
	enum class GridType {
			empty, full, random, glider, pulser, glidergun,
	};
	enum class ColorStyle{
			black = 0,
			white = 0b000001
	};
	
	class Process {
	private:
		struct {
			uint8_t gridCount = 2;
			size_t gridSizeX;
			size_t gridSizeY;
			gol::Status ***grid;
		};
		sf::RectangleShape **rects;
		gol::ColorStyle colorStyle = gol::ColorStyle::black;
	public:
		Process();
		void init_new_grid(gol::GridType &gridtype);
		void update();
		void draw(sf::RenderWindow *window);
	};
}