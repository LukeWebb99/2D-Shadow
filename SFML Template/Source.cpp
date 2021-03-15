#include <iostream>
#include <SFML/Graphics.hpp>
#include "2D Shadow.h"

using namespace std;
using namespace sf;

vector<Line> segments;

int main()
{
	//segments
	{
		//Window Border
		segments.push_back(Line(0, 0, 640, 0));
		segments.push_back(Line(640, 0, 640, 360));
		segments.push_back(Line(640, 360, 0, 360));
		segments.push_back(Line(0, 360, 0, 0));
		// Ploy #1
		segments.push_back(Line(100, 150, 120, 50));
		segments.push_back(Line(120, 50, 200, 80));
		segments.push_back(Line(200, 80, 140, 210));
		segments.push_back(Line(140, 210, 100, 150));
		// Ploy #2
		segments.push_back(Line(100, 200, 120, 250));
		segments.push_back(Line(120, 250, 60, 300));
		segments.push_back(Line(60, 300, 100, 200));
		// Ploy #3
		segments.push_back(Line(200, 260, 220, 150));
		segments.push_back(Line(220, 150, 300, 200));
		segments.push_back(Line(300, 200, 350, 320));
		segments.push_back(Line(350, 320, 200, 260));
		// Ploy #4
		segments.push_back(Line(340, 60, 360, 40));
		segments.push_back(Line(360, 40, 370, 70));
		segments.push_back(Line(370, 70, 340, 60));
		// Ploy #5
		segments.push_back(Line(450, 190, 560, 170));
		segments.push_back(Line(560, 170, 540, 270));
		segments.push_back(Line(540, 270, 430, 290));
		segments.push_back(Line(430, 290, 450, 190));
		// Play #6
		segments.push_back(Line(400, 95, 580, 50));
		segments.push_back(Line(580, 50, 480, 150));
		segments.push_back(Line(480, 150, 400, 95));
	}

	sf::ContextSettings settings; settings.antialiasingLevel = 16; settings.depthBits = 24;
	sf::RenderWindow window(sf::VideoMode(640, 360), "2D Shadows", sf::Style::None, settings);
	window.setActive();
	
	Shadow main(segments);

	while (window.isOpen())
	{

		sf::Vector2i mousepos = sf::Mouse::getPosition(window);

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();

		main.update(sf::Vector2i(mousepos.x, mousepos.y));
		main.draw(window);

		for (int i = 0; i < segments.size(); i++) {
			segments[i].display(window);
		}

		window.display();
	}

	return 0;
}