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

#ifndef GTL_TEST_ALLOCATOR_H
#define GTL_TEST_ALLOCATOR_H

#include <gtl/allocator.h>
#include <gtl/debug.h>

namespace gtl {

struct Test_Allocator : public gtl::Allocator
{
	Test_Allocator() : m_alloc(0), m_dealloc(0) {}

	~Test_Allocator()
	{
		GTL_ASSERT(m_alloc == m_dealloc);
	}

	virtual void* allocate(size_t count)
	{
		++m_alloc;
		return new char[count];
	}

	virtual void deallocate(void* pT)
	{
		if(pT)
		{
			++m_dealloc;
			delete[] (char*) pT;
		}
	}

	int outstanding() const
	{
		return m_alloc - m_dealloc;
	}

	int m_alloc;
	int m_dealloc;
};

}

#endif //include guard

