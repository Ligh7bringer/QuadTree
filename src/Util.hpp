#pragma once

#include <stdexcept>
#include <string>

#define UNIMPL_FAIL()                                                                              \
	throw std::runtime_error("Unimplemented function " + std::string(__PRETTY_FUNCTION__) +        \
							 "\n\tline: " + std::to_string(__LINE__) +                             \
							 "\tfile: " + std::string(__FILE__))

#define DEF_VALUE_GETTER(TYPE, PARAM)                                                              \
	TYPE PARAM() const { return PARAM##_; }

#define DEF_CONST_REF_GETTER(TYPE, PARAM)                                                          \
	const TYPE& PARAM() const { return PARAM##_; }

#define DEF_NON_CONST_REF_GETTER(TYPE, PARAM)                                                      \
	TYPE& PARAM() { return PARAM##_; }

#define DEF_GETTERS(TYPE, PARAM)                                                                   \
	DEF_CONST_REF_GETTER(TYPE, PARAM)                                                              \
	DEF_NON_CONST_REF_GETTER(TYPE, PARAM)

template <typename T>
struct Vec2
{
	T x{0}, y{0};

	Vec2<T>() = default;

	Vec2<T>(T xx, T yy)
		: x(xx)
		, y(yy)
	{ }

	Vec2<T>(T val) { x = val, y = val; }

	Vec2<T>& operator=(const Vec2<T>& rhs)
	{
		if(this == &rhs) return *this;

		x = rhs.x;
		y = rhs.y;
		return *this;
	}

	void zero() { x = T{0}, y = T{0}; }

#define VEC2_DEF_ARITHMETIC_OP(OP)                                                                 \
	Vec2<T> operator OP(const Vec2<T>& rhs) const                                                  \
	{                                                                                              \
		Vec2<T> result;                                                                            \
		result.x = x OP rhs.x;                                                                     \
		result.y = y OP rhs.y;                                                                     \
		return result;                                                                             \
	}

	VEC2_DEF_ARITHMETIC_OP(+)
	VEC2_DEF_ARITHMETIC_OP(-)
	VEC2_DEF_ARITHMETIC_OP(*)
	VEC2_DEF_ARITHMETIC_OP(/)
#undef DEF_ARITHMETIC_OP
};

template <typename T>
constexpr typename std::underlying_type<T>::type to_underlying(T val)
{
	return static_cast<typename std::underlying_type<T>::type>(val);
}