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
#include <gtl/range.h>
#include <list>

namespace gtl {

class Test_Basic_Range : public Gtl_Test_Case
{
public:
	virtual void run(Test_Context& tc)
	{
		{
			int values[] = {0, 1, 2, 3, 4, 5, 6};
			auto range = make_range(values);
			static_assert(is_random_range<decltype(range)>::type::value, "random range test fail");
			auto range2 = range.slice(1, 5);
			GTL_TEST_EQ(tc, range2.size(), 4);
			GTL_TEST_EQ(tc, range2.get(), 1);
			GTL_TEST_EQ(tc, range2.get(3), 4);

			range2 = range2.slice(1, range2.size());
			GTL_TEST_EQ(tc, range2.get(), 2);
		}

		{
			std::list<int> values;
			values.push_back(0);
			values.push_back(1);
			values.push_back(2);
			auto range = make_range(values.begin(), values.end());
			static_assert(!is_random_range<decltype(range)>::type::value, "non random range test fail");

			GTL_TEST_EQ(tc, range.get(), 0);
			range.pop();
			GTL_TEST_EQ(tc, range.get(), 1);
		}

		{
			int values[5] = {0};
			auto rangew = make_range(values);
			auto ranger = rangew;

			for(int i = 0; !rangew.empty(); ++i)
			{
				rangew.set(i);
				rangew.pop();
			}

			GTL_TEST_EQ(tc, ranger.get(), 0);
			GTL_TEST_EQ(tc, ranger.get(3), 3);
		}
	}

private:
};

class Test_Range_Algorithm : public Gtl_Test_Case
{
public:
	template <class Range_T> void test_slice(Test_Context& tc, Range_T range)
	{
		auto range2 = slice(range, 1, 6, 2 /*step*/); //1, 3, 5
		GTL_TEST_EQ(tc, range2.get(), 1);
		range2.pop();
		GTL_TEST_EQ(tc, range2.get(), 3);
		range2.pop();
		GTL_TEST_EQ(tc, range2.get(), 5);
		range2.pop();
		GTL_TEST_VERIFY(tc, range2.empty());
	}

	template <class Range_T> void test_random_slice(Test_Context& tc, Range_T range)
	{
		auto range2 = slice(range, 1, 6, 2 /*step*/); //1, 3, 5
		GTL_TEST_EQ(tc, range2.size(), 3);

		range2 = range2.slice(1, range2.size());
		GTL_TEST_EQ(tc, range2.get(), 3);
		range2.pop();
		GTL_TEST_EQ(tc, range2.get(), 5);
		GTL_TEST_EQ(tc, range2.size(), 1);
	}

	template <class Range_T> void test_truncate(Test_Context& tc, Range_T range)
	{
		auto range2 = truncate_range(range, 4);
		for(size_t i = 0; i < 4; ++i)
		{
			GTL_TEST_VERIFY(tc, !range2.empty());
			range2.pop();
		}

		GTL_TEST_VERIFY(tc, range2.empty());
	}

	template <class Range_T> void test_random_truncate(Test_Context& tc, Range_T range)
	{
		auto range2 = truncate_range(range, 4);
		GTL_TEST_EQ(tc, range2.size(), 4);
	}

	virtual void run(Test_Context& tc)
	{
		int array[] = {0, 1, 2, 3, 4, 5, 6};
		std::list<int> list;

		for(int i = 0; i < 7; ++i) list.push_back(i);

		auto array_range = make_range(array);
		auto list_range = make_range(list.begin(), list.end());

		test_truncate(tc, array_range);
		test_random_truncate(tc, array_range);
		test_slice(tc, list_range);

		test_slice(tc, array_range);
		test_random_slice(tc, array_range);
		test_slice(tc, list_range);
	}
};

void test_range(Test_Platform& platform)
{
	Test_Suite suite("range", platform);

	suite.run("basic", Test_Basic_Range());
	suite.run("algorithm", Test_Range_Algorithm());
}

} //ns
