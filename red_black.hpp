#ifndef _RED_BLACK_HPP_
#define _RED_BLACK_HPP_

#include <memory>
#include <functional>


template <typename T>
class RedBlack
{
public:
	using Comparator = std::function<bool(const T&, const T&)>;
	using Callable = std::function<void(const T&)>;
	enum class TraverseMode {PREORDER, INORDER, POSTORDER};

	class Node
	{
	public:
		enum class ColorType {RED, BLACK} color;

		T& data;

		std::weak_ptr<Node> parent;
		std::unique_ptr<Node> child_left,
							  child_right;

		Node(T& d, ColorType c = ColorType::RED) : color(c), data(d) {}
	};

	std::unique_ptr<Node> root;
	Comparator comparator;

	RedBlack(T& data, Comparator comp = Comparator(std::less<T>())) : root(new Node(data, Node::ColorType::BLACK)), comparator(comp) {}
	RedBlack(const RedBlack& rb) = delete; // TODO kell?
	RedBlack(RedBlack&& rb)
	{
		root.reset(rb.root.release());
		comparator = rb.comparator;
	}

	RedBlack& operator=(const RedBlack& rb) = delete; // TODO kell?


	Node& add(T& d)
	{
		Node& parent = search(d);

		// TODO mi van, ha már létezik d a fában?
		// ez így jó?
		// visszatérek egy olyan Node-al, ami azonos értékű T-t tárol, de nem feltétlenül egyezik meg a két hivatkozott objektum
		if (parent.data == d)
			return parent;

		Node* newNode = new Node(d);
		if (comparator(parent.data, d))
		{
			if (parent.child_right)
				throw std::logic_error("beszurni ugye node ala jobbra, hogy ott mar van valami?");
			else
			{
				parent.child_right.reset(newNode);
			}
		}
		else
		{
			if (parent.child_left)
				throw std::logic_error("beszurni ugye node ala balra, hogy ott mar van valami?");
			else
			{
				parent.child_left.reset(newNode);
			}
		}

		return *newNode;
	}

	Node& search(T& d)
	{
		Node* iterator = root.get();

		bool do_continue;
		do
		{
			do_continue = false;

			// jobbra vagy balra keresünk tovább?
			if (comparator(iterator->data, d))
			{
				//jobbra mennénk tovább
				if (iterator->child_right)
				{
					iterator = iterator->child_right.get();
					do_continue = true;
				}
			}
			else
			{
				// balra mennénk tovább
				if (iterator->child_left)
				{
					iterator = iterator->child_left.get();
					do_continue = true;
				}
			}
		} while (do_continue);

		return *iterator;
	}

	void traverse(Callable func, const TraverseMode mode = TraverseMode::INORDER)
	{
		traverseRecursive(*(root.get()), func, mode);
	}

	void debugDump()
	{
		debugDumpRecursive(root.get(), 0);
	}

private:
	void traverseRecursive(Node& node, Callable func, const TraverseMode mode)
	{
		if (mode == TraverseMode::PREORDER)
			func(node.data);

		if (node.child_left)
			traverseRecursive(*(node.child_left.get()), func, mode);

		if (mode == TraverseMode::INORDER)
			func(node.data);

		if (node.child_right)
			traverseRecursive(*(node.child_right.get()), func, mode);

		if (mode == TraverseMode::POSTORDER)
			func(node.data);
	}

	void debugDumpRecursive(const Node* node, size_t level)
	{
		if(node->child_right)
		{
			std::cout << std::endl;
			debugDumpRecursive(node->child_right.get(), level + 1);
			std::cout << std::endl;
			for(size_t i = 0; i < level; i++)
				std::cout << "    ";
			std::cout << "   /" << std::endl;
		}

		for(size_t i = 0; i < level; i++)
			std::cout << "    ";
		std::cout << '[' << (node->color == Node::ColorType::RED ? 'R' : 'b') << "] " << node->data;

		if(node->child_left)
		{
			std::cout << std::endl;
			for(size_t i = 0; i < level; i++)
				std::cout << "    ";
			std::cout << "   \\" << std::endl;
			debugDumpRecursive(node->child_left.get(), level + 1);
		}

	}
};

/*
template <typename T>
class RedBlack
{
	using Comparator = std::function<bool(const T&, const T&)>;

	// DATA
	enum {
		BLACK, RED
	} Color;

	T& data;
	Comparator comparator;

	std::unique_ptr<RedBlack> child_left, child_right;
	std::weak_ptr<RedBlack> parent;

	RedBlack(const RedBlack&) = delete;
	RedBlack(RedBlack&& rb)
	{
		data = rb.data;
		child_left = std::move(rb.child_left);
		child_right = std::move(rb.child_right);
		parent = rb.parent;
	}
	RedBlack& operator=(const RedBlack&) = delete;
	T& operator * () { return data; }

public:
	static RedBlack<T>& create(T&, Comparator comp = nullptr)
	{
		RedBlack root(T);
		if (!comp)
			root.comparator = Comparator(comp);
		else
			root.comparator = Comparator(std::less<T>());
		return root;
	}

	explicit RedBlack(T& t, Comparator c = nullptr) : data{t}, child_left{nullptr}, child_right{nullptr} {}
};
*/
#endif // _RED_BLACK_HPP_