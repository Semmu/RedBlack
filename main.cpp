#include <iostream>
#include <vector>
#include <list>
#include "red_black.hpp"

// just for testing purposes
struct SomethingDynamic
{
	size_t number = 0;

	SomethingDynamic(size_t n = 0) : number(n) {}
	SomethingDynamic(const SomethingDynamic& sd) : number{sd.number} {}
	SomethingDynamic(SomethingDynamic&& moveable)
	{
		number = moveable.number;
		moveable.number = 0;
	}
};

std::ostream& operator << (std::ostream& os, const SomethingDynamic& var)
{
	os << "SD(" << var.number << ")";
	return os;
}


int main()
{
/*
		      .                               .
		o     |            ,-                 |         o
		. ;-. |-  ,-. ;-.  |  ,-: ,-. ,-.   ,-| ,-. ,-. . ,-: ;-.
		| | | |   |-' |    |- | | |   |-'   | | |-' `-. | | | | |
		' ' ' `-' `-' '    |  `-` `-' `-'   `-' `-' `-' ' `-| ' '
		                  -'                              `-'
*/

	std::vector<int> v_ints({8, 3, 5, 2, 6});
	// static construct method to create a tree from an existing container
	// does not take away resources
//	RedBlack<int> rb_ints = RedBlack::copy_from(v_ints.begin(), v_ints.end());


	SomethingDynamic sd1{9}, sd2{4}, sd3{22}, sd4{18}, sd5{42}, sd6{31};
	std::vector<SomethingDynamic> l_sds;
	l_sds.push_back(sd1);
	l_sds.push_back(sd2);
	l_sds.push_back(sd3);
	l_sds.push_back(sd4);
	l_sds.push_back(sd5);
	l_sds.push_back(sd6);

	for (auto sd : l_sds)
		std::cout << sd << " ";

	std::cout << sd1;

	std::list<SomethingDynamic> l_sds2;
	for (auto sd : l_sds)
		l_sds2.push_back(sd);

	for (auto sd : l_sds)
		std::cout << sd << " ";

	// like the previous, but this one takes away resources (std::move)
//	RedBlack<int> rb_ints = RedBlack::move_from()


	// explicit search, throws exception if not found
	SomeClass found = rbX.search(someclassinsta);
	// using lambda function to test every node
	SomeClass found2 = rbX.search([](SomeClass& node) {return true;});

	// search, but does not throw exception, return the nearest node
	SomeClass found2 = rbX.searchOrNearest(someclassinsta2);


	// trivial
	rbX.add(SomeClass& node);
	rbX.remove(SomeClass& node);

	// calling a function on every node
	rbY.traverse([](SomeClass& node) {std::cout << node.data << std::endl; }, INORDER | PREORDER | POSTORDER);


	return 0;
}