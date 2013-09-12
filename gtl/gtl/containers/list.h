/**
 * Copyright 2013 Kevin Lin
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef GTL_CONTAINERS_LIST_H
#define GTL_CONTAINERS_LIST_H

#include <gtl/common.h>
#include <gtl/context.h>
#include <gtl/type_traits.h>
#include "list_base.h"

namespace gtl {

template <class T> struct List_Node_T : public List_Node_Base
{
	typename std::aligned_storage<
		sizeof(T),
		std::alignment_of<T>::value
	>::type data;

	T* ptr() {return reinterpret_cast<T*>(&data);}
};

template <class T, class Ref, class Ptr>
	struct List_Iterator : public List_Iterator_Base
{
	typedef List_Iterator<T, T&, T*> iterator;
	typedef List_Iterator<T, T const&, T const*> const_iterator;
	typedef List_Iterator<T, Ref, Ptr> self;

	typedef T value_type;
	typedef Ptr pointer;
	typedef Ref reference;
	typedef List_Node_T<T> node_type;

	List_Iterator();
	List_Iterator(List_Node_Base* x) : List_Iterator_Base(x) {}
	List_Iterator(iterator const& iter) : List_Iterator_Base(iter.m_node) {}

	reference operator*() const
	{
		return *static_cast<node_type*>(m_node)->ptr();
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

template <class T, class Base, class Access> class List_Range : public Base
{
public:
	typedef Access* Ptr;
	typedef Access& ref_type;

	typedef T const& get_type;
	typedef T const& set_type;
	typedef List_Node_T<T> node_type;

	List_Range(List_Node_Base* begin, List_Node_Base* end) : Base(begin, end) {}

	template <class Ptr>
	List_Range(List_Range<T, Base, Ptr> const& other) : Base(other)
	{
	}

	ref_type ref() const {return *current_ptr();}
	get_type get() const {return *current_ptr();}
	void set(set_type value) const {*current_ptr() = value;}

private:
	Ptr current_ptr() const {return static_cast<node_type*>(current())->ptr();}
};

template <class T> class List
{
public:
	typedef List_Node_T<T> node_type;

	typedef T value_type;
	typedef value_type* pointer;
	typedef const value_type* const_pointer;
	typedef value_type& reference;
	typedef const value_type& const_reference;
	typedef std::size_t size_type;
	typedef std::ptrdiff_t difference_type;

	typedef List_Iterator<T, T&, T*> iterator;
	typedef List_Iterator<T, T const&, T const*> const_iterator;

	typedef List_Range<T, List_Forward_Range_Base, T> range;
	typedef List_Range<T, List_Forward_Range_Base, T const> const_range;

	typedef List_Range<T, List_Reverse_Range_Base, T> reverse_range;
	typedef List_Range<T, List_Reverse_Range_Base, T const> const_reverse_range;

	List(Context const* context) : m_context(context)
	{
		m_node.next = &m_node;
		m_node.prev = &m_node;
	}

	~List()
	{
		clear();
	}

	iterator begin() {return (node_type*)(m_node.next); }
	const_iterator begin() const { return (node_type*)(m_node.next); }

	iterator end() {return (node_type*)&m_node;}
	//A const cast is implied
	const_iterator end() const {return (node_type*)&m_node;}

	range all()
	{
		return range(
			static_cast<node_type*>(m_node.next),
			static_cast<node_type*>(&m_node));
	}

	const_range all() const
	{
		return const_range(
			static_cast<node_type const*>(m_node.next),
			static_cast<node_type const*>(&m_node));
	}

	reverse_range reverse()
	{
		return reverse_range(
			static_cast<node_type*>(m_node.next),
			static_cast<node_type*>(&m_node));
	}

	const_reverse_range reverse() const
	{
		return const_reverse_range(
			static_cast<node_type const*>(m_node.next),
			static_cast<node_type const*>(&m_node));
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

	void swap(List<T>& x)
	{
		std::swap(m_context, x.m_context);
		std::swap(m_node, x.m_node);
	}
	
	iterator insert(iterator position, T const& x)
	{
		return func_emplace(position, gtl::emplace(x));
	}

	template <class TT>
	iterator emplace(iterator position, TT&& x)
	{
		return func_emplace(position, gtl::emplace(x));
	}

	template <class Emplace_T>
	iterator func_emplace(iterator position, Emplace_T func)
	{
		auto deleter = [this](node_type* p){dealloc_node(p);};
		auto node(scope(alloc_node(), deleter));

		construct(node.get());
		//Now place the actual data
		func(node.get()->ptr());
		insert_before(position.m_node, node.get());
		return node.release();
	}

	template <class Range_T>
	void insert_range(iterator position, Range_T range);

	template <class Input_Iter>
	void insert(iterator position, Input_Iter first, Input_Iter last)
	{
		insert_range(position, make_range(first, last));
	}

	void push_front(T const& x) {func_emplace(begin(), gtl::emplace(x)); }
	void push_back(T const& x) {func_emplace(end(), gtl::emplace(x)); }
	
	template <class TT>
	void emplace_front(TT& x) {func_emplace(begin(), gtl::emplace(x));}

	template <class TT>
	void emplace_back(TT& x) {func_emplace(end(), gtl::emplace(x));}

	template <class Emplace_T>
	void func_emplace_front(Emplace_T func) {insert(begin(), func);}

	template <class Emplace_T>
	void func_emplace_back(Emplace_T func) {insert(end(), func);}

	iterator erase(iterator position)
	{
		node_type* node = static_cast<node_type*>(position.m_node);
		List_Node_Base* next_node = gtl::erase(position.m_node);
		destruct(node);
		dealloc_node(node);
		return iterator(static_cast<node_type*>(next_node));
	}

	iterator erase(iterator first, iterator last);

	void pop_front() {erase(begin());}

	void pop_back()
	{ 
		iterator tmp = end();
		erase(--tmp);
	}

	void clear();

	void splice(iterator position, List& x)
	{
		verify_transferrable(x);
		if (!x.empty()) 
			transfer(position.m_node, x.begin().m_node, x.end().m_node);
	}

	void splice(iterator position, List& x, iterator i)
	{
		verify_transferrable(x);
		iterator j = i;
		++j;
		if (position == i || position == j) return;
		transfer(position.m_node, i.m_node, j.m_node);
	}

	void splice(iterator position, List& x, iterator first, iterator last)
	{
		verify_transferrable(x);
		if (first != last) 
			transfer(position.m_node, first.m_node, last.m_node);
	}

private:
	List(List const&);
	List& operator=(List const&);

	node_type* alloc_node()
	{
		return static_cast<node_type*>(m_context->allocator->allocate(sizeof(node_type)));
	}

	void dealloc_node(node_type* p)
	{
		m_context->allocator->deallocate(p);
	}

	void verify_transferrable(List& other)
	{
		if(m_context->allocator != other.m_context->allocator)
		{
			raise_exception(Exception("nodes cannot be transferred"));
		}
	}

private:
	Context const* m_context;
	List_Node_Base m_node;
};

template <class T>
template <class Range_T>
void List<T>::insert_range(iterator position, Range_T range)
{
	for(; !range.empty(); range.pop())
	{
		insert(position, range.get());
	}
}

template <class T>
typename List<T>::iterator List<T>::erase(iterator first, iterator last)
{
	while (first != last)
		erase(first++);
	return last;
}

template <class T>
void List<T>::clear()
{
	List_Node_Base* current = m_node.next;
	while(current != &m_node)
	{
		node_type* node = static_cast<node_type*>(current);
		current = current->next;
		destruct(node);
		dealloc_node(node);
	}

	m_node.next = &m_node;
	m_node.prev = &m_node;
}

} //namespace


#endif

