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


#ifndef GTL_RANGE_DETAIL_RANGE_ADAPTER_H
#define GTL_RANGE_DETAIL_RANGE_ADAPTER_H

#include <gtl/range/range.h>

namespace gtl {
	
	namespace detail {

template <class Range_T> class Range_Adapter
{
protected:
	Range_T m_range;

public:
	Range_Adapter(Range_T const& range) : m_range(range) {}

	typedef typename Range_T::get_type get_type;
	typedef typename Range_T::set_type set_type;

	get_type get() const {return m_range.get();}
	void set(set_type value) const {m_range.set(value);}
	void pop() {m_range.pop();}

	//random
	get_type get(size_t i) const {return m_range.get(i);}
	void set(size_t i, set_type value) const {m_range.set(i, value);}
	Range_T slice(size_t begin, size_t end) {m_range.slice(begin, end);}
	size_t size() const {return m_range.size();}

	bool empty() const {return m_range.empty();}

	auto begin() -> decltype(m_range.begin()) const {return m_range.begin();}
	auto end() -> decltype(m_range.end()) const {return m_range.end();}
};

template <class Range_T> class Truncate_Range : public Range_Adapter<Range_T>
{
public:
	Truncate_Range(Range_T const& range, size_t truncate) :
		Range_Adapter<Range_T>(range),
		m_truncate(truncate), m_count(0) {}

	void pop()
	{
		m_range.pop();
		++m_count;
	}

	bool empty() const
	{
		return m_count >= m_truncate || m_range.empty();
	}

private:
	size_t m_truncate;
	size_t m_count;
};

template <class Range_T> struct Slice_Range : public Range_Adapter<Range_T>
{
	typedef typename Range_T::get_type get_type;
	typedef typename Range_T::set_type set_type;

	Slice_Range(Range_T range, size_t begin, size_t end, size_t step) :
		Range_Adapter<Range_T>(range),
		m_size(end - begin),
		m_step(step)
	{
		for(; !m_range.empty() && begin > 0; --begin)
		{
			m_range.pop();
		}
	}

	get_type get() const
	{
		return m_range.get();
	}

	void pop()
	{
		for(size_t i = 0; !m_range.empty() && i < m_step; ++i)
		{
			m_range.pop();
		}

		--m_size;
	}

	bool empty() const
	{
		return m_size == 0 || m_range.empty();
	}

private:
	size_t m_size;
	size_t m_step;
};

template <class Range_T> class Random_Slice_Range : public Range_Adapter<Range_T>
{
public:
	typedef typename Range_T::get_type get_type;
	typedef typename Range_T::set_type set_type;

	Random_Slice_Range(Range_T range, size_t begin, size_t end, size_t step) :
		Range_Adapter<Range_T>(range),
		m_begin(begin),
		m_end(end),
		m_step(step)
	{
		//Fix up the end by rounding up to the intervals
		size_t intervals = (end - begin + step - 1) / step;
		m_end = begin + intervals * step;
	}

	get_type get() const
	{
		return m_range.get(m_begin);
	}

	void pop()
	{
		m_begin += m_step;
	}

	get_type get(size_t i) const
	{
		return m_range.get(i * m_step + m_begin);
	}

	void set(size_t i, set_type value) const
	{
		m_range.set(i * m_step + m_begin, value);
	}

	Random_Slice_Range slice(size_t begin, size_t end)
	{
		begin = m_begin + begin * m_step;
		end = m_begin + end * m_step;

		return Random_Slice_Range(m_range, begin, end, m_step);
	}

	size_t size() const
	{
		return (m_end - m_begin) / m_step;
	}

	bool empty() const {return m_begin >= m_end;}

private:
	size_t m_begin;
	size_t m_end;
	size_t m_step;
};

	} //detail

template <class Range_T> struct is_random_range<detail::Random_Slice_Range<Range_T>>
{
	typedef true_type type;
};

} //ns

#endif
