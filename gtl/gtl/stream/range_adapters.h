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

#ifndef GTL_STREAM_RANGE_ADAPTERS_H
#define GTL_STREAM_RANGE_ADAPTERS_H

#include <gtl/common.h>
#include "stream.h"

namespace gtl {

template <class T, class Range_T> class Range_Input_Stream :
	public Input_Stream<T>
{
public:
	Range_Input_Stream(Range_T const& range) : m_range(range) {}

	virtual T get()
	{
		T ret =  m_range.get();
		m_range.pop();
		return ret;
	}

	virtual bool eof() const {return m_range.empty();}

private:
	Range_T m_range;
};

template <class T, class Range_T> class Range_Output_Stream :
	public Output_Stream<T>
{
public:
	Range_Output_Stream(Range_T const& range) : m_range(range) {}

	virtual void put(T data)
	{
		m_range.set(data);
		m_range.pop();
	}

	virtual bool eof() const {return m_range.empty();}

private:
	Range_T m_range;
};

template <class T, class Range_T>
Range_Input_Stream<T, Range_T> range_in_stream(Range_T const& range)
{
	return Range_Input_Stream<T, Range_T>(range);
}

template <class T, class Range_T>
Range_Output_Stream<T, Range_T> range_out_stream(Range_T const& range)
{
	return Range_Output_Stream<T, Range_T>(range);
}

} //ns

#endif

