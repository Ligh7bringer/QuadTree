#pragma once

#include "Util.hpp"

class Boundary
{
	Vec2<float> centre_, size_;

public:
	Boundary() = default;

	Boundary(const Vec2<float>& centre, const Vec2<float>& size)
		: centre_(centre)
		, size_(size)
	{ }

	bool contains(const Boundary& other) const
	{
		return (other.centre().x > centre_.x - size_.x && other.centre().x < centre_.x + size_.x) &&
			   (other.centre().y > centre_.y - size_.y && other.centre().y < centre_.y + size_.y);
	}

	DEF_CONST_REF_GETTER(Vec2<float>, centre)
	DEF_CONST_REF_GETTER(Vec2<float>, size)
};
