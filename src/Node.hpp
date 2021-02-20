#pragma once

#include <memory>
#include <utility>
#include <vector>

#include "Boundary.hpp"
#include "Util.hpp"

template <typename T, int Capacity>
class Node
{
	using QuadList = std::array<std::unique_ptr<Node>, 4>;

	enum class Subquad : size_t
	{
		NE = 0,
		NW,
		SE,
		SW,
		NUM_SUBQUADS
	};

	QuadList subquads_;
	Boundary bound_;

	std::array<T, Capacity> values_;
	std::array<Boundary, Capacity> value_bounds_;

	uint32_t num_values_;
	bool divided_;
	bool external_;

	// FIXME: Values should be propagated down to child nodes. Currently they are kept in the
	// parent node *as well as* inserted into the child nodes.
	void subdivide()
	{
		if(!divided_)
		{
			const auto child_size = size() / 2.f;

			auto maybe_insert_into_subquad = [&](auto subquad_idx) -> void {
				for(size_t i = 0; i < num_values_; ++i)
				{
					if(subquads_[subquad_idx]->boundary().contains(value_bounds_[i]))
					{
						subquads_[subquad_idx]->insert(value_bounds_[i], values_[i]);
					}
				}
			};

			const auto NE_pos =
				Vec2<float>(centre().x + size().x / 2.f, centre().y - size().y / 2.f);
			subquads_[to_underlying(Subquad::NE)].reset(
				new Node<T, Capacity>(Boundary(NE_pos, child_size)));
			maybe_insert_into_subquad(to_underlying(Subquad::NE));

			const auto NW_pos =
				Vec2<float>(centre().x - size().x / 2.f, centre().y - size().y / 2.f);
			subquads_[to_underlying(Subquad::NW)].reset(
				new Node<T, Capacity>(Boundary(NW_pos, child_size)));
			maybe_insert_into_subquad(to_underlying(Subquad::NW));

			const auto SE_pos =
				Vec2<float>(centre().x + size().x / 2.f, centre().y + size().y / 2.f);
			subquads_[to_underlying(Subquad::SE)].reset(
				new Node<T, Capacity>(Boundary(SE_pos, child_size)));
			maybe_insert_into_subquad(to_underlying(Subquad::SE));

			const auto SW_pos =
				Vec2<float>(centre().x - size().x / 2.f, centre().y + size().y / 2.f);
			subquads_[to_underlying(Subquad::SW)].reset(
				new Node<T, Capacity>(Boundary(SW_pos, child_size)));
			maybe_insert_into_subquad(to_underlying(Subquad::SW));

			divided_ = true;
			external_ = false;
		}
	}

public:
	Node() = default;

	explicit Node(const Boundary& bound)
		: bound_(bound)
		, num_values_(0)
		, divided_(false)
		, external_(true)
	{ }

	bool insert(Boundary bound, const T& value)
	{
		if(!bound_.contains(bound)) return false;

		if(num_values_ < Capacity)
		{
			value_bounds_[num_values_] = bound;
			values_[num_values_++] = value;
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
				if(subquads_[i]->boundary().contains(bound))
				{
					if(subquads_[i]->insert(bound, value)) return true;
				}
			}
		}

		return false;
	}

	template <typename Func>
	void for_each_value(Func&& func)
	{
		for(size_t i = 0; i < num_values_; ++i)
		{
			func(value_bounds_[i], values_[i]);
		}

		if(divided())
		{
			for(size_t i = 0; i < to_underlying(Subquad::NUM_SUBQUADS); ++i)
			{
				subquads_[i]->for_each_value(func);
			}
		}
	}

	template <typename Func>
	void for_each_node(Func&& func)
	{
		func(this);

		if(divided())
		{
			for(size_t i = 0; i < to_underlying(Subquad::NUM_SUBQUADS); ++i)
			{
				subquads_[i]->for_each_node(func);
			}
		}
	}

	bool empty() const { return num_values_ == 0; }

	const Boundary& boundary() const { return bound_; }

	const Vec2<float>& centre() const { return bound_.centre(); }

	const Vec2<float>& size() const { return bound_.size(); }

	DEF_VALUE_GETTER(bool, divided)
	DEF_VALUE_GETTER(bool, external)
};
