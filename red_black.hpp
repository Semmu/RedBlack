#ifndef _RED_BLACK_HPP_
#define _RED_BLACK_HPP_

#include <memory>

template <typename T>
class RedBlack
{
	std::shared_ptr<T> data;
	std::unique_ptr<RedBlack> child_left, child_right;
	std::weak_ptr<RedBlack> parent;

	enum {
		BLACK, RED
	} Color;

public:
	RedBlack(T& t) : data{&t}, child_left{nullptr}, child_right{nullptr} {}
	T& operator * () { return data; }
};

#endif // _RED_BLACK_HPP_