#pragma once

#include <cmath>

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "Util.hpp"

class Body
{
	Vec2<float> position_;
	Vec2<float> velocity_;
	float mass_;
	Vec2<float> force_;
	sf::CircleShape sprite_;

public:
	Body() = default;

	Body(const Vec2<float>& position, const Vec2<float>& velocity, float mass)
		: position_(position)
		, velocity_(velocity)
		, mass_(mass)
		, sprite_(sf::CircleShape(5.f))
	{
		sprite_.setFillColor(sf::Color::Yellow);
		sprite_.setPosition({position_.x, position_.y});
		sprite_.setOrigin({sprite_.getRadius() / 2.f, sprite_.getRadius() / 2.f});
	}

	void render(sf::RenderWindow& window) { window.draw(sprite_); }

	void update(float dt) { UNIMPL_FAIL(); }

	float distance_to(const Body& other) const
	{
		auto delta = position_ - other.position();
		return std::sqrt(delta.x * delta.x + delta.y * delta.y);
	}

	void reset_force() { force_.zero(); }

	void add_force(const Body& other) { UNIMPL_FAIL(); }

	DEF_GETTERS(Vec2<float>, position)
	DEF_GETTERS(Vec2<float>, velocity)
	DEF_GETTERS(Vec2<float>, force)
	DEF_GETTERS(float, mass)
};
