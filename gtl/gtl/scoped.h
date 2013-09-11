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

#ifndef GTL_SCOPED_H
#define GTL_SCOPED_H

#include "common.h"
#include <algorithm> //for swap

namespace gtl {

template <class DataT, class FuncT> class Scoped;

template <class DataT, class FuncT> class Scoped
{
	//Used for releasing into a container
	struct Release_Emplace
	{
		Release_Emplace(Scoped& scoped) : m_scoped(scoped) {}

		void operator()(DataT* other)
		{
			new (other) DataT(m_scoped.release());
		}

		Scoped& m_scoped;
	};

public:
	typedef DataT data_type;
	typedef FuncT func_type;

	Scoped(func_type const& func) :
		m_bound(false),
		m_func(func)
	{}

	Scoped(data_type const& data, func_type const& func) :
		m_bound(true),
		m_data(data),
		m_func(func)
	{
	}
	
	//Move
	Scoped(Scoped&& other) :
		m_bound(other.m_bound),
		m_data(other.m_data),
		m_func(other.m_func)
	{
		other.m_bound = false;
	}

	~Scoped()
	{
		if(m_bound)
		{
			m_func(m_data);
		}
	}

	data_type const& get() const
	{
		return m_data;
	}

	bool valid() const
	{
		return m_bound;
	}

	void reset()
	{
		Scoped temp(m_func);
		swap(temp);
	}

	void reset(data_type const& data)
	{
		Scoped temp(data, m_func);
		swap(temp);
	}

	data_type release()
	{
		m_bound = false;
		return m_data;
	}

	Release_Emplace release_emplace()
	{
		return Release_Emplace(*this);
	}

private:
	void swap(Scoped& other)
	{
		std::swap(m_bound, other.m_bound);
		std::swap(m_data, other.m_data);
	}

protected:
	bool m_bound;
	data_type m_data;
	func_type const m_func;

private:
	Scoped(Scoped const& other);
	Scoped& operator=(Scoped const& other);
};

template <class FuncT> class Scoped<void, FuncT>
{
public:
	typedef FuncT func_type;

	Scoped(func_type const& func) :
		m_bound(true),
		m_func(func)
	{}

	//Move
	Scoped(Scoped&& other) :
		m_bound(other.m_bound),
		m_func(other.m_func)
	{
		other.m_bound = false;
	}

	~Scoped()
	{
		reset();
	}

	bool valid() const
	{
		return m_bound;
	}

	void reset()
	{
		if(m_bound)
		{
			m_func();
		}
	}

	void release()
	{
		m_bound = false;
	}

protected:
	bool m_bound;
	func_type const m_func;

private:
	Scoped(Scoped const& other);
	Scoped& operator=(Scoped const& other);
};


template <class DataT, class FuncT>
Scoped<DataT, FuncT> scope(DataT data, FuncT func)
{
	return Scoped<DataT, FuncT>(data, func);
}

template <class FuncT>
Scoped<void, FuncT> scope(FuncT func)
{
	return Scoped<void, FuncT>(func);
}

//Simple scope guard, use GTL_SCOPE to execute a piece of function (likely lambda)
//on scope exit
#define GTL_SCOPE(func) auto GTL_CONCAT(_scope_, __LINE__)(scope(func));

} //gtl

#endif

