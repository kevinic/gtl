/**
 * Adapted by Kevin Lin. (c) 2009.
 *
 * Copyright (c) 1994
 * Hewlett-Packard Company
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Hewlett-Packard Company makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 *
 * Copyright (c) 1996,1997
 * Silicon Graphics Computer Systems, Inc.
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Silicon Graphics makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 */

#ifndef GTL_CONTAINERS_ILIST_H
#define GTL_CONTAINERS_ILIST_H

#include <gtl/common.h>
#include "list_base.h"

namespace gtl {

template <class Tag> struct Ilist_Node_T : public List_Node_Base
{
};

typedef Ilist_Node_T<void> Ilist_Node;

template <class T, class Tag, class Ref, class Ptr>
	struct Ilist_Iterator : public List_Iterator_Base
{
	typedef Ilist_Iterator<T, Tag, T&, T*> iterator;
	typedef Ilist_Iterator<T, Tag, T const&, T const*> const_iterator;
	typedef Ilist_Iterator<T, Tag, Ref, Ptr> self;

	typedef T value_type;
	typedef Ptr pointer;
	typedef Ref reference;
	typedef Ilist_Node_T<Tag> node_t;

	Ilist_Iterator();
	Ilist_Iterator(node_t* x) : List_Iterator_Base(x) {}
	Ilist_Iterator(iterator const& iter) : List_Iterator_Base(iter.m_node) {}

	reference operator*() const
	{
		return *static_cast<pointer>(static_cast<node_t*>(m_node));
	}

	pointer operator->() const {return &(operator*());}

	self& operator++()
	{
		this->incr();
		return *this;
	}

	self operator++(int)
	{
		self temp = *this;
		this->incr();
		return temp;
	}

	self& operator--()
	{
		this->decr();
		return *this;
	}

	self operator--(int)
	{
		self temp = *this;
		this->decr();
		return temp;
	}
};

template <class T, class Base, class Access, class Tag> class Ilist_Range : public Base
{
public:
	typedef Access* Ptr;
	typedef Access& ref_type;

	typedef T const& get_type;
	typedef T const& set_type;
	typedef Ilist_Node_T<Tag> node_type;

	Ilist_Range(List_Node_Base* begin, List_Node_Base* end) : Base(begin, end) {}

	template <class Ptr>
	Ilist_Range(Ilist_Range<T, Base, Ptr, Tag> const& other) : Base(other)
	{
	}

	ref_type ref() const {return *current_ptr();}
	get_type get() const {return *current_ptr();}
	void set(set_type value) {*current_ptr() = value;}

private:
	Ptr current_ptr() const
	{
		return static_cast<Ptr>(static_cast<node_type*>(current()));
	}
};

template <class T, class Tag = void> class Ilist
{
public:
	typedef Ilist_Node_T<Tag> node_t;
	typedef T value_type;
	typedef value_type* pointer;
	typedef const value_type* const_pointer;
	typedef value_type& reference;
	typedef const value_type& const_reference;
	typedef std::size_t size_type;
	typedef std::ptrdiff_t difference_type;

	typedef Ilist_Iterator<T, Tag, T&, T*> iterator;
	typedef Ilist_Iterator<T, Tag, T const&, T const*> const_iterator;

	typedef Ilist_Range<T, List_Forward_Range_Base, T, Tag> range;
	typedef Ilist_Range<T, List_Forward_Range_Base, T const, Tag> const_range;

	typedef Ilist_Range<T, List_Reverse_Range_Base, T, Tag> reverse_range;
	typedef Ilist_Range<T, List_Reverse_Range_Base, T const, Tag> const_reverse_range;

	Ilist()
	{
		m_node.next = &m_node;
		m_node.prev = &m_node;
	}

	iterator begin() {return (node_t*)(m_node.next); }
	const_iterator begin() const { return (node_t*)(m_node.next); }

	iterator end() {return (node_t*)&m_node;}
	//A const cast is implied
	const_iterator end() const {return (node_t*)&m_node;}

	range all()
	{
		return range(
			static_cast<node_t*>(m_node.next),
			static_cast<node_t*>(&m_node));
	}

	const_range all() const
	{
		//Const cast and promote cast
		return const_range(
			(node_t*)(m_node.next),
			(node_t*)(&m_node));
	}

	reverse_range reverse()
	{
		return reverse_range(
			static_cast<node_t*>(m_node.next),
			static_cast<node_t*>(&m_node));
	}

	const_reverse_range reverse() const
	{
		//Const cast and promote cast
		return const_reverse_range(
			(node_t*)(m_node.next),
			(node_t*)(&m_node));
	}

	bool empty() const { return m_node.next == &m_node; }

	size_type size() const {
		size_type result = std::distance(begin(), end());
		return result;
	}

	size_type max_size() const { return size_type(-1); }

	reference front() { return *begin(); }
	const_reference front() const { return *begin(); }
	reference back() { return *(--end()); }
	const_reference back() const { return *(--end()); }

	iterator get_iterator(T& x)
	{
		return iterator(static_cast<node_t*>(&x));
	}

	void swap(Ilist<T, Tag>& x) {std::swap(m_node, x.m_node);}

	iterator insert(iterator position, T& x)
	{
		node_t* node = &x;
		insert_before(position.m_node, node);
		return node;
	}

	template <class Range_T>
	void insert_range(iterator position, Range_T range);

	template <class Input_Iter>
	void insert(iterator position, Input_Iter first, Input_Iter last)
	{
		insert_range(position, make_range(first, last));
	}

	void push_front(T& x) {insert(begin(), x); }
	void push_back(T& x) {insert(end(), x); }

	iterator erase(iterator position)
	{
		List_Node_Base* next_node = gtl::erase(position.m_node);
		return iterator(static_cast<node_t*>(next_node));
	}

	iterator erase(T& x)
	{
		return erase(iterator(static_cast<node_t*>(&x)));
	}

	iterator erase(iterator first, iterator last);

	void pop_front() {erase(begin());}

	void pop_back()
	{ 
		iterator tmp = end();
		erase(--tmp);
	}

	void clear()
	{
		m_node.next = &m_node;
		m_node.prev = &m_node;
	}

	void splice(iterator position, Ilist& x)
	{
		if (!x.empty()) 
			transfer(position.m_node, x.begin().m_node, x.end().m_node);
	}

	void splice(iterator position, Ilist&, iterator i) {
		iterator j = i;
		++j;
		if (position == i || position == j) return;
		transfer(position.m_node, i.m_node, j.m_node);
	}

	void splice(iterator position, Ilist&, iterator first, iterator last) {
		if (first != last) 
			transfer(position.m_node, first.m_node, last.m_node);
	}

private:
	Ilist(Ilist const&);
	Ilist& operator=(Ilist const&);

private:
	node_t m_node;
};

template <class T, class Tag>
template <class Range_T>
void Ilist<T, Tag>::insert_range(iterator position, Range_T range)
{
	for(; !range.empty(); range.pop())
	{
		insert(position, range.ref());
	}
}

template <class T, class Tag>
typename Ilist<T, Tag>::iterator Ilist<T, Tag>::erase(iterator first, iterator last)
{
	while (first != last)
		erase(first++);
	return last;
}

} //namespace

#endif

