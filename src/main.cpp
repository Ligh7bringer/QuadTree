#include <iostream>
#include <memory>
#include <random>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

#include "Body.hpp"
#include "QuadTree.hpp"
#include "Util.hpp"

int main()
{
	const Vec2<float> window_size(1000.f, 1000.f);

	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	sf::RenderWindow window(sf::VideoMode(window_size.x, window_size.y),
							"Quad Tree",
							sf::Style::Close | sf::Style::Titlebar,
							settings);
	sf::View view(sf::FloatRect(0, 0, window_size.x, window_size.y));
	view.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));
	window.setView(view);

	QuadTree tree(window_size / 2.f, window_size / 2.f);

	// std::random_device rd{};
	// std::mt19937 gen(rd());
	// std::uniform_real_distribution<float> width_distr(0.f, window_size.x);
	// std::uniform_real_distribution<float> height_distr(0.f, window_size.y);

	// for(size_t i = 0; i < 1000; ++i)
	// {
	// 	auto body = std::make_shared<Body>(
	// 		Vec2<float>(width_distr(gen), height_distr(gen)), Vec2<float>(10.f, 10.f), 10.f);
	// 	tree.insert(body);
	// }

	while(window.isOpen())
	{
		sf::Event event;
		while(window.pollEvent(event))
		{
			if(event.type == sf::Event::Closed) window.close();

			if(event.type == sf::Event::MouseWheelScrolled)
			{
				view.zoom(1.f + (-event.mouseWheelScroll.delta / 10.f));
			}

			if(event.type == sf::Event::MouseButtonPressed)
			{
				if(event.mouseButton.button == sf::Mouse::Left)
				{
					auto body = std::make_shared<Body>(
						Vec2<float>(event.mouseButton.x, event.mouseButton.y),
						Vec2<float>(10.f, 10.f),
						10.f);
					tree.insert(body);
				}
			}
		}

		window.clear(sf::Color::Black);

		tree.render(window);

		window.setView(view);
		window.display();
	}

	return EXIT_SUCCESS;
}