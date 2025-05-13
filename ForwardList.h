#pragma once

#include <cstddef>
#include <iterator>
#include <stdexcept>
#include <utility>
#include <initializer_list>
#include <vector>

template <typename _Ty>
class ForwardList
{
private:
	struct Node
	{
		_Ty _value;
		Node* _next;

		Node(const _Ty& value, Node* next = nullptr)
			: _value(value), _next(next) {
		}
	};

	Node* _head;

	void copy_from(const ForwardList& other);

public:
	using value_type = _Ty;
	using reference = value_type&;
	using const_reference = const value_type&;
	using pointer = value_type*;
	using const_pointer = const value_type*;
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;


	class Iterator
	{
	private:
		Node* _current;
		friend class ForwardList;

	public:
		using iterator_category = std::forward_iterator_tag;
		using value_type = ForwardList::value_type;
		using reference = ForwardList::reference;
		using pointer = ForwardList::pointer;
		using difference_type = ForwardList::difference_type;

		Iterator(Node* node) : _current(node) {}
		~Iterator() = default;

		reference operator*() const { return _current->_value; }
		pointer operator->() const { return &_current->_value; }

		Iterator& operator++()
		{
			_current = _current->_next;
			return *this;
		}

		Iterator operator++(int)
		{
			Iterator temp = *this;
			++(*this);
			return temp;
		}

		bool operator==(const Iterator& other) const { return _current == other._current; }
		bool operator!=(const Iterator& other) const { return _current != other._current; }
	};

	class Const_Iterator
	{
	private:
		Node* _current;
		friend class ForwardList;

	public:
		using iterator_category = std::forward_iterator_tag;
		using value_type = ForwardList::value_type;
		using reference = const value_type&;
		using pointer = const value_type*;
		using difference_type = ForwardList::difference_type;

		Const_Iterator(Node* node) : _current(node) {}
		~Const_Iterator() = default;

		const_reference operator*() const { return _current->_value; }
		const_pointer operator->() const { return &_current->_value; }

		Const_Iterator& operator++()
		{
			_current = _current->_next;
			return *this;
		}

		Const_Iterator operator++(int)
		{
			Const_Iterator temp = *this;
			++(*this);
			return temp;
		}

		bool operator==(const Const_Iterator& other) const { return _current == other._current; }
		bool operator!=(const Const_Iterator& other) const { return _current != other._current; }
	};

	using iterator = Iterator;
	using const_iterator = Const_Iterator;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;


	ForwardList() noexcept;
	ForwardList(const ForwardList& other);
	ForwardList(ForwardList&& other) noexcept;
	ForwardList(std::initializer_list<value_type> init);
	~ForwardList() noexcept;

	ForwardList& operator=(const ForwardList& other);
	ForwardList& operator=(ForwardList&& other) noexcept;

	void assign(std::initializer_list<value_type> init);
	void clear() noexcept;

	void push_front(const_reference value);
	template <typename... Args>
	void emplace_front(Args&&... args);

	void pop_front();

	reference front();
	const_reference front() const;

	bool empty() const noexcept;
	size_type size() const noexcept;

	void swap(ForwardList& other) noexcept;

	void erase_after(size_type position);
	void erase_after(iterator pos);
	iterator insert_after(iterator pos, const_reference value);

	iterator begin() { return iterator{ _head }; }
	iterator end() { return iterator{ nullptr }; }
	const_iterator begin() const { return const_iterator{ _head }; }
	const_iterator end() const { return const_iterator{ nullptr }; }
	const_iterator cbegin() const { return const_iterator{ _head }; }
	const_iterator cend() const { return const_iterator{ nullptr }; }

	reverse_iterator rbegin() { return reverse_iterator(end()); }
	reverse_iterator rend() { return reverse_iterator(begin()); }
	const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
	const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }
	const_reverse_iterator crbegin() const { return const_reverse_iterator(cend()); }
	const_reverse_iterator crend() const { return const_reverse_iterator(cbegin()); }

	iterator find(const_reference value);
	const_iterator find(const_reference value) const;
	bool contains(const_reference value) const;
	std::vector<value_type> to_vector() const;

	void reverse() noexcept;
	void unique();
	void sort();
	void merge(ForwardList& other);

	friend bool operator==(const ForwardList& lhs, const ForwardList& rhs)
	{
		Node* a = lhs._head;
		Node* b = rhs._head;
		while (a && b)
		{
			if (a->_value != b->_value)
				return false;
			a = a->_next;
			b = b->_next;
		}
		return a == nullptr && b == nullptr;
	}

	friend bool operator!=(const ForwardList& lhs, const ForwardList& rhs)
	{
		return !(lhs == rhs);
	}
};

// ===== Реализация методов =====

template<typename _Ty>
void ForwardList<_Ty>::copy_from(const ForwardList& other)
{
	if (!other._head)
	{
		_head = nullptr;
		return;
	}

	_head = new Node(other._head->_value);
	Node* current = _head;
	Node* other_current = other._head->_next;
	while (other_current)
	{
		current->_next = new Node(other_current->_value);
		current = current->_next;
		other_current = other_current->_next;
	}
}

template<typename _Ty>
ForwardList<_Ty>::ForwardList() noexcept : _head(nullptr) {}

template<typename _Ty>
ForwardList<_Ty>::ForwardList(const ForwardList& other) { copy_from(other); }

template<typename _Ty>
ForwardList<_Ty>::ForwardList(ForwardList&& other) noexcept : _head(other._head) { other._head = nullptr; }

template<typename _Ty>
ForwardList<_Ty>::ForwardList(std::initializer_list<value_type> init) : _head(nullptr)
{
	for (auto it = std::rbegin(init); it != std::rend(init); ++it)
		push_front(*it);
}

template<typename _Ty>
ForwardList<_Ty>::~ForwardList() noexcept { clear(); }

template<typename _Ty>
ForwardList<_Ty>& ForwardList<_Ty>::operator=(const ForwardList& other)
{
	if (this != &other)
	{
		clear();
		copy_from(other);
	}
	return *this;
}

template<typename _Ty>
ForwardList<_Ty>& ForwardList<_Ty>::operator=(ForwardList&& other) noexcept
{
	if (this != &other)
	{
		clear();
		_head = other._head;
		other._head = nullptr;
	}
	return *this;
}

template<typename _Ty>
void ForwardList<_Ty>::assign(std::initializer_list<value_type> init)
{
	clear();
	for (auto it = std::rbegin(init); it != std::rend(init); ++it)
		push_front(*it);
}

template<typename _Ty>
void ForwardList<_Ty>::clear() noexcept
{
	while (_head)
	{
		Node* temp = _head;
		_head = _head->_next;
		delete temp;
	}
}

template<typename _Ty>
void ForwardList<_Ty>::push_front(const_reference value)
{
	_head = new Node(value, _head);
}

template<typename _Ty>
template<typename... Args>
void ForwardList<_Ty>::emplace_front(Args&&... args)
{
	_head = new Node(_Ty(std::forward<Args>(args)...), _head);
}

template<typename _Ty>
void ForwardList<_Ty>::pop_front()
{
	if (!_head)
		throw std::runtime_error("ForwardList is empty");
	Node* temp = _head;
	_head = _head->_next;
	delete temp;
}

template<typename _Ty>
typename ForwardList<_Ty>::reference ForwardList<_Ty>::front()
{
	if (!_head)
		throw std::runtime_error("ForwardList is empty");
	return _head->_value;
}

template<typename _Ty>
typename ForwardList<_Ty>::const_reference ForwardList<_Ty>::front() const
{
	if (!_head)
		throw std::runtime_error("ForwardList is empty");
	return _head->_value;
}

template<typename _Ty>
bool ForwardList<_Ty>::empty() const noexcept
{
	return _head == nullptr;
}

template<typename _Ty>
typename ForwardList<_Ty>::size_type ForwardList<_Ty>::size() const noexcept
{
	size_type count = 0;
	Node* current = _head;
	while (current)
	{
		++count;
		current = current->_next;
	}
	return count;
}

template<typename _Ty>
void ForwardList<_Ty>::swap(ForwardList& other) noexcept
{
	std::swap(_head, other._head);
}

template<typename _Ty>
void ForwardList<_Ty>::erase_after(size_type position)
{
	Node* current = _head;
	for (size_type i = 0; i < position; ++i)
	{
		if (!current)
			throw std::out_of_range("Position out of range");
		current = current->_next;
	}
	if (current && current->_next)
	{
		Node* temp = current->_next;
		current->_next = temp->_next;
		delete temp;
	}
	else
	{
		throw std::out_of_range("Nothing to erase after this position");
	}
}

template<typename _Ty>
void ForwardList<_Ty>::erase_after(iterator pos)
{
	if (!pos._current || !pos._current->_next)
		throw std::out_of_range("Nothing to erase after this iterator");
	Node* temp = pos._current->_next;
	pos._current->_next = temp->_next;
	delete temp;
}

template<typename _Ty>
typename ForwardList<_Ty>::iterator ForwardList<_Ty>::insert_after(iterator pos, const_reference value)
{
	if (!pos._current)
		throw std::out_of_range("Invalid iterator");
	Node* new_node = new Node(value, pos._current->_next);
	pos._current->_next = new_node;
	return iterator(new_node);
}

template<typename _Ty>
typename ForwardList<_Ty>::iterator ForwardList<_Ty>::find(const_reference value)
{
	Node* current = _head;
	while (current)
	{
		if (current->_value == value)
			return iterator(current);
		current = current->_next;
	}
	return end();
}

template<typename _Ty>
typename ForwardList<_Ty>::const_iterator ForwardList<_Ty>::find(const_reference value) const
{
	Node* current = _head;
	while (current)
	{
		if (current->_value == value)
			return const_iterator(current);
		current = current->_next;
	}
	return end();
}

template<typename _Ty>
bool ForwardList<_Ty>::contains(const_reference value) const
{
	return find(value) != end();
}

template<typename _Ty>
std::vector<typename ForwardList<_Ty>::value_type> ForwardList<_Ty>::to_vector() const
{
	std::vector<value_type> result;
	Node* current = _head;
	while (current)
	{
		result.push_back(current->_value);
		current = current->_next;
	}
	return result;
}

template<typename _Ty>
void ForwardList<_Ty>::reverse() noexcept {
	Node* prev = nullptr;
	Node* current = _head;
	while (current) {
		Node* next = current->_next;
		current->_next = prev;
		prev = current;
		current = next;
	}
	_head = prev;
}

template<typename _Ty>
void ForwardList<_Ty>::unique() {
	if (!_head) return;

	Node* current = _head;
	while (current->_next) {
		if (current->_value == current->_next->_value) {
			Node* temp = current->_next;
			current->_next = temp->_next;
			delete temp;
		}
		else {
			current = current->_next;
		}
	}
}

template<typename _Ty>
void ForwardList<_Ty>::sort() 
{
	if (!_head || !_head->_next) 
		return;

	// Bottom-up merge sort
	auto merge_lists = [](Node* a, Node* b) -> Node* 
		{
			Node dummy(_Ty{});
			Node* tail = &dummy;

			while (a && b) 
			{
				if (a->_value <= b->_value) 
				{
					tail->_next = a;
					a = a->_next;
				}
				else 
				{
					tail->_next = b;
					b = b->_next;
				}
				tail = tail->_next;
			}
			tail->_next = a ? a : b;
			return dummy._next;
		};

	size_type list_size = 1;
	while (true) 
	{
		Node* result = nullptr;
		Node** last_ptr = &result;

		Node* remaining = _head;
		bool did_merge = false;

		while (remaining) 
		{
			did_merge = true;
			Node* left = remaining;
			Node* right = nullptr;

			for (size_type i = 1; i < list_size && remaining; ++i)
				remaining = remaining->_next;

			if (remaining) 
			{
				right = remaining->_next;
				remaining->_next = nullptr;
				remaining = right;
			}

			for (size_type i = 1; i < list_size && remaining; ++i)
				remaining = remaining->_next;

			Node* next = nullptr;
			if (remaining) 
			{
				next = remaining->_next;
				remaining->_next = nullptr;
			}

			*last_ptr = merge_lists(left, right);
			while (*last_ptr)
				last_ptr = &((*last_ptr)->_next);

			remaining = next;
		}

		if (!did_merge) 
			break;
		_head = result;
		list_size *= 2;
	}
}

template<typename _Ty>
void ForwardList<_Ty>::merge(ForwardList& other) 
{
	if (this == &other) 
		return;

	Node dummy(_Ty{});
	Node* tail = &dummy;
	Node* a = _head;
	Node* b = other._head;

	while (a && b) 
	{
		if (a->_value <= b->_value) 
		{
			tail->_next = a;
			a = a->_next;
		}
		else {
			tail->_next = b;
			b = b->_next;
		}
		tail = tail->_next;
	}

	tail->_next = a ? a : b;
	_head = dummy._next;
	other._head = nullptr;
}

