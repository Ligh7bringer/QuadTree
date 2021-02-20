#pragma once

#include <memory>

#include <SFML/Graphics/RenderWindow.hpp>

#include "Boundary.hpp"
#include "Node.hpp"

template <typename T, int NodeCapacity = 1>
class QuadTree
{
	std::unique_ptr<Node<T, NodeCapacity>> root_;

public:
	QuadTree() = delete;

	explicit QuadTree(const Boundary& bound) { root_.reset(new Node<T, NodeCapacity>(bound)); }

	void insert(const Boundary& bound, const T& value) { root_->insert(bound, value); }

	template <typename Func>
	void for_each_value(Func&& func)
	{
		root_->for_each_value(func);
	}

	template <typename Func>
	void for_each_node(Func&& func)
	{
		root_->for_each_node(func);
	}
};
