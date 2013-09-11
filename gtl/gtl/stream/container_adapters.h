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

#ifndef GTL_STREAM_CONTAINER_ADAPTERS_H
#define GTL_STREAM_CONTAINER_ADAPTERS_H

#include <gtl/common.h>
#include "stream.h"
#include "range_adapters.h"

namespace gtl {

template <class T, class Container_T> class Container_Output_Stream :
	public Output_Stream<T>
{
public:
	Container_Output_Stream(Container_T& container) : m_container(container) {}

	virtual void put(T data) {m_container.push_back(data);}
	virtual bool eof() const {return false;}

private:
	Container_T& m_container;
};

template <class T, class Container_T>
auto container_in_stream(Container_T& container) -> decltype(range_in_stream<T>(container.all()))
{
	return range_in_stream<T>(container.all());
}

template <class T, class Container_T>
Container_Output_Stream<T, Container_T>
container_out_stream(Container_T& container)
{
	return Container_Output_Stream<T, Container_T>(container);
}

} //ns

#endif
