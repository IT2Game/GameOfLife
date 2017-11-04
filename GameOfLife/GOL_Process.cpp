#include"GOL_Process.h"
#include<random>
#include<vector>
#include<thread>
#include<mutex>

uint8_t gol::Process::getNaigbourCount(uint32_t x, uint32_t y, uint32_t gridIndex)
{
	this->countMutex.lock(); 
	uint32_t naightbours = 0;
	for (uint32_t u_x = 0; u_x < 3; u_x++) {
		for (int u_y = 0; u_y < 3; u_y++) {
			int pos_x = x + u_x - 1;
			int pos_y = y + u_y - 1;

			if (pos_x < 0)
				pos_x = this->gridSizeX - 1;

			if (pos_x > this->gridSizeX - 1)
				pos_x = 0;

			if (pos_y < 0)
				pos_y = this->gridSizeY - 1;

			if (pos_y > this->gridSizeY - 1)
				pos_y = 0;

			switch (this->grid[gridIndex][pos_x][pos_y])
			{
			case gol::Status::life:
				naightbours++;
			default:
				break;
			}
		}
	}
	this->countMutex.unlock();
	return naightbours;
}

gol::Process::Process(size_t width, size_t height, sf::RenderWindow *window)
{
	this->colorStyle = gol::ColorStyle::black;
	this->gridSizeX = width;
	this->gridSizeY = height;

	this->secondaryUpdators.resize(std::thread::hardware_concurrency() - 1);

	this->grid = new gol::Status**[this->gridCount];
	for (uint32_t x = 0; x < this->gridCount; x++) {
		this->grid[x] = new gol::Status*[this->gridSizeX];
		for (uint32_t y = 0; y < this->gridSizeX; y++) {
			this->grid[x][y] = new gol::Status[this->gridSizeY];
		}
	}

	sf::Vector2f size;
	size.x = static_cast<float>(window->getSize().x) / static_cast<float>(this->gridSizeX);
	size.y = static_cast<float>(window->getSize().y) / static_cast<float>(this->gridSizeY);

	this->rects = new sf::RectangleShape*[this->gridSizeX];
	for (int x = 0; x < this->gridSizeX; x++) {
		this->rects[x] = new sf::RectangleShape[this->gridSizeY];
		for (int y = 0; y < this->gridSizeY; y++) {
			
			this->rects[x][y].setSize(size);
			this->rects[x][y].setPosition(x * size.x, y * size.y);
			this->rects[x][y].setFillColor(sf::Color(0, 0, 0));
		}
	}

	this->init_new_grid(gol::GridType::random);
}

void gol::Process::init_new_grid(gol::GridType gridtype)
{
	if (gridtype == gol::GridType::random) {
		for (int x = 0; x < this->gridSizeX; x++) {
			for (int y = 0; y < this->gridSizeY; y++) {
				this->grid[actualGrid][x][y] = (gol::Status)(std::rand() % 2);
				this->rects[x][y].setFillColor((grid[actualGrid][x][y] == gol::Status::life) ? sf::Color(0, 0, 0) : sf::Color(255, 255, 255) );
			}
		}
	}
	
}
void gol::Process::update()
{
	uint32_t actualThread = 0;
	uint8_t nextGrid = (actualGrid + 1) % this->gridCount;
	uint8_t actualGrid = this->actualGrid;
	for (int x = 0; x < this->gridSizeX; x++) {
		this->secondaryUpdators[actualThread] = std::thread([=]() {
			for (uint32_t y = 0; y < this->gridSizeY; y++) {
				uint32_t naigbours = this->getNaigbourCount(x, y, actualGrid);

				switch (naigbours) {
				case 2:
					grid[nextGrid][x][y] = grid[actualGrid][x][y];
					break;
				case 3:
					grid[nextGrid][x][y] = gol::Status::life;
					break;
				default:
					grid[nextGrid][x][y] = gol::Status::dead;
					break;
				}

				if (grid[actualGrid][x][y] == gol::Status::life) {
					rects[x][y].setFillColor(sf::Color(0, 0, 0));
				}
				else {
					rects[x][y].setFillColor(sf::Color(255, 255, 255));
				}
			}
		});
		actualThread = (actualThread + 1) % secondaryUpdators.size();
		if (secondaryUpdators[actualThread].joinable())
			secondaryUpdators[actualThread].join();
	}
	for (int i = 0; i < secondaryUpdators.size(); i++) {
		if (secondaryUpdators[i].joinable())
			secondaryUpdators[i].join();
	}

	this->actualGrid = nextGrid;
}

void gol::Process::draw(sf::RenderWindow * window)
{
	for (uint32_t x = 0; x < this->gridSizeX; x++) {
		for (uint32_t y = 0; y < this->gridSizeY; y++) {
			window->draw(this->rects[x][y]);
		}
	}
}

gol::Process::~Process()
{
	for (int x = 0; x < this->gridCount-1; x++) {
		for (int y = 0; y < this->gridSizeX; y++) {
			delete[] this->grid[x][y];
		}
		delete[] this->grid[x];
	}
	delete[] this->grid;

	for (int x = 0; x < this->gridSizeX; x++) {
		delete[] this->rects[x];
	}
	delete[] this->rects;
}
