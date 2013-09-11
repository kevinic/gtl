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

#ifndef GTL_STREAM_STREAM_ADAPTERS_H
#define GTL_STREAM_STREAM_ADAPTERS_H

#include <gtl/common.h>
#include "stream.h"

namespace gtl {

template <class T, class Stream, class Func> class Adapter_Input_Stream :
	public Input_Stream<T>
{
public:
	Adapter_Input_Stream(Stream& stream, Func const& func) :
		m_stream(stream), m_func(func) {}

	virtual T get() override
	{
		return m_func(m_stream.get());
	}

	virtual bool eof() const override {return m_stream.eof();}

private:
	Stream& m_stream;
	Func m_func;
};

template <class T, class Stream, class Func> class Adapter_Output_Stream :
	public Output_Stream<T>
{
public:
	Adapter_Output_Stream(Stream& stream, Func const& func) :
		m_stream(stream), m_func(func) {}

	virtual void put(T data) override
	{
		m_stream.put(m_func(data));
	}

	virtual bool eof() const override {return m_stream.eof();}

private:
	Stream& m_stream;
	Func m_func;
};

template <class T, class Stream, class Func>
Adapter_Input_Stream<T, Stream, Func> adapter_in_stream(Stream& stream, Func const& func)
{
	return Adapter_Input_Stream<T, Stream, Func>(stream, func);
}

template <class T, class Stream, class Func>
Adapter_Output_Stream<T, Stream, Func> adapter_out_stream(Stream& stream, Func const& func)
{
	return Adapter_Output_Stream<T, Stream, Func>(stream, func);
}

} //ns

#endif
