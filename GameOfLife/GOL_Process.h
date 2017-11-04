#pragma once
#include<SFML\Graphics.hpp>
#include<mutex>
#include<vector>
#include<thread>
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
			sf::RectangleShape **rects;
		};
		std::mutex countMutex;
		std::vector<std::thread> secondaryUpdators;
		uint8_t actualGrid = 0;
		gol::ColorStyle colorStyle = gol::ColorStyle::black;

	public:
		Process(size_t width, size_t height, sf::RenderWindow* window);
		uint8_t getNaigbourCount(uint32_t x, uint32_t y, uint32_t gridIndex);
		void init_new_grid(gol::GridType gridtype);
		void update();
		void draw(sf::RenderWindow *window);
		~Process();
	};
}