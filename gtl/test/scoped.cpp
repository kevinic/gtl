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
#include <gtl/string.h>
#include <gtl/scoped.h>

namespace gtl {

class Test_Scoped : public Gtl_Test_Case
{
public:
	Test_Scoped() : m_count(0) {}

	int create(int count)
	{
		m_count += count;
		return count;
	}

	virtual void run(Test_Context& tc)
	{
		auto deleter = [&](int count) {m_count -= count;};
		{
			int count = create(3);
			auto scoped(scope(count, deleter));
			GTL_TEST_EQ(tc, m_count, 3);
			GTL_TEST_EQ(tc, scoped.get(), 3);
		}

		GTL_TEST_EQ(tc, m_count, 0);

		{
			int count = create(2);
			auto scoped(scope(count, deleter));
			GTL_TEST_VERIFY(tc, scoped.valid());
			int count2 = scoped.release();
			GTL_TEST_EQ(tc, count2, 2);
			GTL_TEST_VERIFY(tc, !scoped.valid());
		}

		GTL_TEST_EQ(tc, m_count, 2);
		m_count = 0;

		{
			int count = create(3);
			auto scoped(scope(count, deleter));
			scoped.reset(create(2));
			GTL_TEST_VERIFY(tc, scoped.valid());
			GTL_TEST_EQ(tc, scoped.get(), 2);
			GTL_TEST_EQ(tc, m_count, 2);

			scoped.reset();
			GTL_TEST_VERIFY(tc, !scoped.valid());
			GTL_TEST_EQ(tc, m_count, 0);
		}
	}

private:
	int m_count;
};


void test_scoped(Test_Platform& platform)
{
	Test_Suite suite("scoped", platform);

	Test_Scoped test_scoped;
	suite.run("scoped", test_scoped);
}

} //ns
