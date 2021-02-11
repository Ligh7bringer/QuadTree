#pragma once

#include <memory>

#include <SFML/Graphics/RenderWindow.hpp>

#include "Body.hpp"
#include "Quad.hpp"

class QuadTree
{
	std::unique_ptr<Quad> root_;

public:
	QuadTree() = delete;

	QuadTree(const Vec2<float> root_centre, const Vec2<float>& root_size)
	{
		root_.reset(new Quad(root_centre, root_size));
	}

	void insert(std::shared_ptr<Body> body) { root_->insert(body); }

	void render(sf::RenderWindow& window) { root_->render(window); }
};
