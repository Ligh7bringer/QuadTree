#include <iostream>
#include <memory>
#include <random>

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

#include "Body.hpp"
#include "Boundary.hpp"
#include "Node.hpp"
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

	QuadTree<Body, 1> tree(Boundary(window_size / 2.f, window_size / 2.f));

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
					const Boundary bound(Vec2<float>(event.mouseButton.x, event.mouseButton.y),
										 Vec2<float>(5.f, 5.f));
					const Body body(Vec2<float>(event.mouseButton.x, event.mouseButton.y),
									Vec2<float>(10.f, 10.f),
									10.f);
					tree.insert(bound, body);
				}
			}
		}

		window.clear(sf::Color::Black);

		tree.for_each_node([&](auto node) -> void {
			const auto& bound = node->boundary();
			auto sprite = sf::RectangleShape({bound.size().x * 2.f, bound.size().y * 2.f});
			sprite.setFillColor(sf::Color::Transparent);
			sprite.setOutlineThickness(-2.f);
			sprite.setOutlineColor(sf::Color::White);
			sprite.setPosition({bound.centre().x, bound.centre().y});
			sprite.setOrigin(sprite.getSize().x / 2, sprite.getSize().y / 2);
			window.draw(sprite);
		});

		tree.for_each_value([&](const Boundary& bound, const auto& value) -> void {
			(void)value;
			auto sprite = sf::CircleShape(5.f);
			sprite.setFillColor(sf::Color::Yellow);
			sprite.setPosition({bound.centre().x, bound.centre().y});
			sprite.setOrigin({sprite.getRadius() / 2.f, sprite.getRadius() / 2.f});
			window.draw(sprite);
		});

		window.setView(view);
		window.display();
	}

	return EXIT_SUCCESS;
}