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
#include <gtl/pool.h>

namespace gtl {

class Test_Node_Pool : public Gtl_Test_Case
{
public:
	struct Counter
	{
		Counter() : constructed(0), destructed(0) {}

		int constructed;
		int destructed;
	};

	struct Node
	{
		Node(Counter& counter) : m_counter(counter)
		{
			m_counter.constructed++;
		}

		~Node()
		{
			m_counter.destructed++;
		}

		Node& operator=(int i)
		{
			m_data = i;
			return *this;
		}

		operator int()
		{
			return m_data;
		}

		int m_data;
		Counter& m_counter;
	};

	template <class T, class Func_T> void test(Test_Context& tc, Func_T func)
	{
		Node_Pool<T> pool(&m_context, 5);
		T* ptr[5];
		for(int i = 0; i < 5; ++i)
		{
			GTL_TEST_VERIFY(tc, !pool.empty());
			ptr[i] = pool.create(func);
			*ptr[i] =  i;
		}
			
		GTL_TEST_VERIFY(tc, pool.empty());
			
		//Read back and destroy
		for(int i = 0; i < 5; ++i)
		{
			GTL_TEST_EQ(tc, (int) *ptr[i], i);
			pool.destroy(ptr[i]);
		}
	}

	virtual void run(Test_Context& tc)
	{
		test<int>(tc, emplace());
		test<char>(tc, emplace());

		Counter counter;
		test<Node>(tc, emplace(counter));
		
		GTL_TEST_EQ(tc, counter.constructed, 5);
		GTL_TEST_EQ(tc, counter.destructed, 5);
	}
};

void test_pool(Test_Platform& platform)
{
	Test_Suite suite("pool", platform);

	Test_Node_Pool test;
	suite.run("node pool", test);
}

} //ns
