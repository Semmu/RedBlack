#ifndef _RED_BLACK_HPP_
#define _RED_BLACK_HPP_

#include <memory>
#include <functional>

#define DEBUG(x) {std::cout << x << std::endl;}

template <typename T>
class RedBlack
{
public:
	using Comparator = std::function<bool(const T&, const T&)>;
	using Callable = std::function<void(const T&)>;
	enum class TraverseMode {PREORDER, INORDER, POSTORDER};
	enum class RotationDirection {LEFT, RIGHT};

	class Node
	{
	public:
		enum class ColorType {RED, BLACK} color;

		T& data;

		std::weak_ptr<Node> parent;
		std::shared_ptr<Node> child_left,
							  child_right;

		Node(T& d, ColorType c = ColorType::RED) : color(c), data(d) {}
	};

	std::shared_ptr<Node> root;
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
		auto parent = search(d);
		std::shared_ptr<Node> newNode(new Node(d));
		if (comparator(parent.get()->data, d))
			parent.get()->child_right = newNode;
		else
			parent.get()->child_left = newNode;

		newNode.get()->parent = parent;

		return *newNode;

/*		auto parent = search(d);

		// TODO mi van, ha már létezik d a fában?
		// ez így jó?
		// visszatérek egy olyan Node-al, ami azonos értékű T-t tárol, de nem feltétlenül egyezik meg a két hivatkozott objektum
		/*if (parent.data == d)
			return parent;

		auto newNode = std::shared_ptr<Node>(new Node(d));

		// szivat ez a sor :(
//		newNode.get()->parent = parent;
		if (comparator(parent.get()->data, d))
			parent.get()->child_right = newNode;
		else
			parent.get()->child_left = newNode;

		return *(newNode.get());*/
	}

	/*void rotate(Node& n, RotationDirection dir)
	{
		switch(dir)
		{
			case RotationDirection::LEFT:
			{
				Node* root_was = &n;
				Node* root_will_be = root_was->child_right.get();
				Node* to_be_adopted = root_will_be->child_left.get();

				Node* root_was_parent = root_was->parent.lock().get();

				DEBUG("wat")

				// adopt the child
				DEBUG("wtf")
				root_was->child_right.reset(to_be_adopted);
				DEBUG("whyyy")
				to_be_adopted->parent = std::shared_ptr<Node>(root_was);

				DEBUG("adopted");

				// change root and root will be order
				if (root_was_parent != nullptr)
				{
					RotationDirection which_child = (root_was_parent->child_right.get() == root_was ? RotationDirection::RIGHT : RotationDirection::LEFT);
					if (which_child == RotationDirection::RIGHT)
					{
						root_was_parent->child_right.reset(root_will_be);
					}
					else
					{
						root_was_parent->child_left.reset(root_will_be);
					}

					root_will_be->parent = std::shared_ptr<Node>(root_was_parent);
				}

				root_will_be->child_left.reset(root_was);
				root_was->parent = std::shared_ptr<Node>(root_will_be);

			}
			break;

			case RotationDirection::RIGHT:
			break;
		}
	}*/

	std::shared_ptr<Node> search(T& d)
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

		return std::shared_ptr<Node>(iterator);
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