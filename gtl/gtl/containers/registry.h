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

#ifndef GTL_CONTAINERS_REGISTRY_H
#define GTL_CONTAINERS_REGISTRY_H

#include <gtl/common.h>
#include <gtl/containers/vector.h>
#include <gtl/range.h>

namespace gtl { namespace registry {

template <class T, class Index = unsigned int> class Node
{
public:
	typedef Index index_t;
	typedef Vector<Node*> Child_Array;
	//Yields Node*
	typedef typename Child_Array::const_range range;

	Node(Context const* context) :
		m_context(context),
		m_children(context)
	{}

	~Node()
	{
		clear();
	}

	void clear()
	{
		for(range all = m_children.all(); !all.empty(); all.pop())
		{
			if(Node* node = all.get())
			{
				m_context->destroy(node);
			}
		}

		m_children.clear();
	}

	T& data() {return m_data;}
	T const& data() const {return m_data;}

	index_t count() const {return m_children.size();}

	Node* try_get(index_t index) const
	{
		return index < m_children.size() ? m_children[index] : NULL;
	}

	Node* get(index_t index)
	{
		if(index >= m_children.size())
		{
			m_children.resize(std::max(index + 1, static_cast<index_t>(index * 1.5f)));
		}

		Node* node = m_children[index];
		if(node == NULL)
		{
			node = new (m_context) Node(m_context);
			m_children[index] = node;
		}

		return node;
	}

	Node& operator[](index_t index) {return *get(index);}

	template <class Range_T> Node* get_path(Range_T path)
	{
		Node* current = this;

		for(; !path.empty(); path.pop())
		{
			current = current->get(path.get());
		}

		return current;
	}

	template <class Range_T> Node const* try_get_path(Range_T path) const
	{
		Node const* current = this;

		for(; current && !path.empty(); path.pop())
		{
			current = current->try_get(path.get());
		}

		return current;
	}

	range children() const
	{
		return m_children.all();
	}

private:
	Context const* m_context;
	T m_data;
	Child_Array m_children;
};

template <class Index, size_t N> class Fixed_Path
{
public:
	typedef Index index_t;

	Fixed_Path() : m_len(0) {}

	Fixed_Path& append(index_t index)
	{
		if(m_len < N)
		{
			m_path[m_len] = index;
			++m_len;
		}
		else
		{
			GTL_ASSERT(false);
		}

		return *this;
	}

	Iterator_Range<index_t const*> get() const
	{
		return make_range(m_path, m_path + m_len);
	}

	Iterator_Range<index_t*> get()
	{
		return make_range(m_path, m_path + m_len);
	}

private:
	size_t m_len;
	index_t m_path[N];
};

}} //namespace

#endif //include guard
