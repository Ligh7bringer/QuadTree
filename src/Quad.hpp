#pragma once

#include <memory>
#include <utility>
#include <vector>

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "Body.hpp"
#include "Util.hpp"

class Quad
{
	using QuadList = std::array<std::unique_ptr<Quad>, 4>;

	enum class Subquad : size_t
	{
		NE = 0,
		NW,
		SE,
		SW,
		NUM_SUBQUADS
	};

	std::shared_ptr<Body> body_;
	// Centre of this quad
	Vec2<float> centre_;
	// Width and height, i.e distance from centre to any of the sides
	Vec2<float> size_;
	QuadList subquads_;
	bool divided_;
	bool empty_;
	bool external_;
	sf::RectangleShape sprite_;

	void subdivide()
	{
		if(!divided_)
		{
			const auto size = size_ / 2.f;

			const auto NE_pos = Vec2<float>(centre_.x + size_.x / 2.f, centre_.y - size_.y / 2.f);
			subquads_[to_underlying(Subquad::NE)].reset(new Quad(NE_pos, size));
			if(subquads_[to_underlying(Subquad::NE)]->contains(body_->position()))
			{
				subquads_[to_underlying(Subquad::NE)]->insert(body_);
			}

			const auto NW_pos = Vec2<float>(centre_.x - size_.x / 2.f, centre_.y - size_.y / 2.f);
			subquads_[to_underlying(Subquad::NW)].reset(new Quad(NW_pos, size));
			if(subquads_[to_underlying(Subquad::NW)]->contains(body_->position()))
			{
				subquads_[to_underlying(Subquad::NW)]->insert(body_);
			}

			const auto SE_pos = Vec2<float>(centre_.x + size_.x / 2.f, centre_.y + size_.y / 2.f);
			subquads_[to_underlying(Subquad::SE)].reset(new Quad(SE_pos, size));
			if(subquads_[to_underlying(Subquad::SE)]->contains(body_->position()))
			{
				subquads_[to_underlying(Subquad::SE)]->insert(body_);
			}

			const auto SW_pos = Vec2<float>(centre_.x - size_.x / 2.f, centre_.y + size_.y / 2.f);
			subquads_[to_underlying(Subquad::SW)].reset(new Quad(SW_pos, size));
			if(subquads_[to_underlying(Subquad::SW)]->contains(body_->position()))
			{
				subquads_[to_underlying(Subquad::SW)]->insert(body_);
			}

			divided_ = true;
			external_ = false;
		}
	}

public:
	Quad() = default;

	Quad(const Vec2<float>& centre, const Vec2<float>& size)
		: body_(nullptr)
		, centre_(centre)
		, size_(size)
		, divided_(false)
		, empty_(true)
		, external_(true)
		, sprite_(sf::RectangleShape({size_.x * 2.f, size_.y * 2.f}))
	{
		sprite_.setFillColor(sf::Color::Transparent);
		sprite_.setOutlineThickness(-2.f);
		sprite_.setOutlineColor(sf::Color::White);
		sprite_.setPosition({centre_.x, centre_.y});
		sprite_.setOrigin(sprite_.getSize().x / 2, sprite_.getSize().y / 2);
	}

	void render(sf::RenderWindow& window)
	{
		window.draw(sprite_);

		if(divided())
		{
			for(size_t i = 0; i < to_underlying(Subquad::NUM_SUBQUADS); ++i)
			{
				subquads_[i]->render(window);
			}
		}

		if(!empty() && !divided())
		{
			body_->render(window);
		}
	}

	bool insert(std::shared_ptr<Body> body)
	{
		if(!contains(body->position())) return false;

		if(empty())
		{
			body_ = body;
			empty_ = false;
			return true;
		}
		else
		{
			if(!divided())
			{
				subdivide();
			}

			for(size_t i = 0; i < to_underlying(Subquad::NUM_SUBQUADS); ++i)
			{
				if(subquads_[i]->contains(body->position()))
				{
					if(subquads_[i]->insert(body)) return true;
				}
			}
		}

		return false;
	}

	bool contains(const Vec2<float>& position)
	{
		const auto half_size = size_ / 2.f;
		// Are positive y coordinates up or down?
		return (position.x > centre_.x - size_.x && position.x < centre_.x + size_.x) &&
			   (position.y > centre_.y - size_.y && position.y < centre_.y + size_.y);
	}

	DEF_CONST_REF_GETTER(Vec2<float>, size)
	DEF_VALUE_GETTER(bool, divided)
	DEF_VALUE_GETTER(bool, empty)
	DEF_VALUE_GETTER(bool, external)
};
