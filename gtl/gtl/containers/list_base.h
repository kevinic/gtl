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

#ifndef GTL_CONTAINERS_LIST_BASE_H
#define GTL_CONTAINERS_LIST_BASE_H

#include <gtl/common.h>
#include <gtl/range.h>
#include <iterator>

namespace gtl {

struct List_Node_Base
{
	List_Node_Base() {}
	List_Node_Base* prev;
	List_Node_Base* next;

//noncopyable
private:
	List_Node_Base(List_Node_Base const&);
	List_Node_Base& operator=(List_Node_Base const&);
};

struct List_Iterator_Base
{
	typedef std::size_t size_type;
	typedef std::ptrdiff_t difference_type;
	typedef std::bidirectional_iterator_tag iterator_category;

	List_Iterator_Base(List_Node_Base* node) : m_node(node) {}
	List_Iterator_Base() : m_node(0) {}

	void incr() {m_node = m_node->next;}
	void decr() {m_node = m_node->prev;}

	bool operator==(List_Iterator_Base const& other) const
	{
		return m_node == other.m_node;
	}

	bool operator!=(List_Iterator_Base const& other) const
	{
		return m_node != other.m_node;
	}

	List_Node_Base* m_node;
};

struct List_Forward_Range_Base
{
	List_Forward_Range_Base(List_Node_Base* begin, List_Node_Base* end) : m_begin(begin), m_end(end) {}

	List_Node_Base* current() const {return m_begin;}

	void pop()
	{
		m_begin = m_begin->next;
	}

	bool empty() const {return m_begin == m_end;}

	List_Node_Base* m_begin;
	List_Node_Base* m_end;
};

struct List_Reverse_Range_Base
{
	List_Reverse_Range_Base(List_Node_Base* begin, List_Node_Base* end) : m_begin(begin), m_end(end) {}

	List_Node_Base* current() const {return m_end->prev;}

	void pop()
	{
		m_end = m_end->prev;
	}

	bool empty() const {return m_begin == m_end;}

	List_Node_Base* m_begin;
	List_Node_Base* m_end;
};

inline void insert_before(List_Node_Base* position, List_Node_Base* node)
{
	node->next = position;
	node->prev = position->prev;
	position->prev->next = node;
	position->prev = node;
}

inline List_Node_Base* erase(List_Node_Base* position)
{
	List_Node_Base* next_node = position->next;
	List_Node_Base* prev_node = position->prev;
	prev_node->next = next_node;
	next_node->prev = prev_node;
	return next_node;
}

inline void transfer(List_Node_Base* position, List_Node_Base* first, List_Node_Base* last)
{
	if(position != last)
	{
		// Remove [first, last) from its old position.
		last->prev->next = position;
		first->prev->next = last;
		position->prev->next = first; 

		// Splice [first, last) into its new position.
		List_Node_Base* tmp = position->prev;
		position->prev = last->prev;
		last->prev = first->prev; 
		first->prev = tmp;
	}
}

} //namespace

#endif

