#pragma once

#include "Util.hpp"

class Body
{
	// FIXME: boundary handles this
	Vec2<float> position_;
	Vec2<float> velocity_;

	float mass_;
	Vec2<float> force_;

public:
	Body() = default;

	Body(const Vec2<float>& position, const Vec2<float>& velocity, float mass)
		: position_(position)
		, velocity_(velocity)
		, mass_(mass)
	{ }

	void update(float dt) { STUB(); }

	void reset_force() { force_.zero(); }

	float distance_to(const Body& other) const
	{
		const auto delta = position_ - other.position();
		return delta.magnitute();
	}

	void add_force(const Body& other) { STUB(); }

	DEF_GETTERS(Vec2<float>, position)
	DEF_GETTERS(Vec2<float>, velocity)
	DEF_GETTERS(Vec2<float>, force)
	DEF_GETTERS(float, mass)
};
