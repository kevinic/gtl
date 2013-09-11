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

#include "common.h"
#include <gtl/context.h>
#include <gtl/debug.h>
#include <vector>
#include <list>

namespace gtl {

struct construct_throw
{
	construct_throw()
	{
		int i = 0;
		throw(i);
	}

	~construct_throw()
	{
	}
};

//test stateful allocation
bool test_allocator()
{
	{
		Test_Allocator alloc;
		gtl::Context context(&alloc);

		try
		{
			construct_throw* pTest = new(&context) construct_throw();
			context.destroy(pTest);
		}
		catch(...)
		{
		}

		GTL_ASSERT(alloc.m_alloc == 1);
		GTL_ASSERT(alloc.m_dealloc == 1);
	}

	//test stl allocator
	{
		Test_Allocator alloc;

		typedef std::list<int, gtl::STL_Allocator<int> > list;
		list mylist(alloc);
		mylist.push_back(5);
		mylist.push_back(5);
		GTL_ASSERT(alloc.m_alloc > 0);
	}

	{
		Test_Allocator alloc;

		typedef std::vector<int, gtl::STL_Allocator<int> > vector;
		vector vec(alloc);
		vec.push_back(5);
		vec.push_back(5);
		GTL_ASSERT(alloc.m_alloc > 0);
	}

	return true;
}

} //gtl
