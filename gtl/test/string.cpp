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

using namespace gtl::string;

namespace gtl {

class Test_Char_Range : public Gtl_Test_Case
{
public:
	template <class Char_T>
	void test(Test_Context& tc, Char_T* string)
	{
		typedef Char_Range_T<Char_T> range_type;
		typedef Char_Range_T<Char_T const> range_const_type;

		range_type empty;
		GTL_TEST_VERIFY(tc, empty.size() == 0);

		{
			range_type range(string);
			range_type range2(string, string + 1);
			range_const_type crange(range);

			GTL_TEST_EQ(tc, range.size(), gtl::string::strlen(string));
			GTL_TEST_VERIFY(tc, range.size() == crange.size());
			GTL_TEST_VERIFY(tc, range2.size() == 1);
		}

		//compilation error!
		{
			//range_const_type crange(string);
			//range_type range(crange);
		}

		{
			range_type range(string);
			GTL_TEST_EQ(tc, range.compare(string), 0);
			GTL_TEST_EQ(tc, range.compare(range_type(string, string + 1)), 1);
			GTL_TEST_EQ(tc, range_type(string, string + 1).compare(range), -1);
		}
	
	}

	virtual void run(Test_Context& tc)
	{
		char buffer[20];
		strlcpy(buffer, "test string", 20);
		test(tc, buffer);

		wchar_t wbuffer[20];
		strlcpy(wbuffer, L"test string", 20);
		test(tc, wbuffer);

		{
			Char_Range range(buffer);
			GTL_TEST_EQ(tc, range.compare("aaa"), 1);
			GTL_TEST_EQ(tc, range.compare("zzz"), -1);

			GTL_TEST_EQ(tc, range.slice(0, range.size()).compare("test string"), 0);
			GTL_TEST_EQ(tc, range.slice(1, 4).compare("est"), 0);
		}

		{
			Char_Range range(buffer);
			GTL_TEST_EQ(tc, range.find("est"), 1);
			GTL_TEST_EQ(tc, range.find("esz"), range.size());
			GTL_TEST_EQ(tc, range.find("test string string"), range.size());
		}

		{
			GTL_TEST_VERIFY(tc, is_ch_class('a', "abc"));
			GTL_TEST_VERIFY(tc, !is_ch_class('d', "abc"));

			GTL_TEST_VERIFY(tc, !is_ch_class('d', Char_Range("abc")));
		}

		{
			char buffer2[20];
			strlcpy(buffer2, "blah", 20);

			replace_ch(buffer2, "ah", 'e');
			GTL_TEST_EQ(tc, compare(buffer2, "blee"), 0);
		}
	}
};

class Test_Builder : public Gtl_Test_Case
{
public:
	virtual void run(Test_Context& tc)
	{
		char buffer[20];
		auto builder(make_builder(buffer));
		GTL_TEST_EQ(tc, gtl::string::strlen(builder.c_str()), 0);
		builder.append("12345");

		GTL_TEST_EQ(tc, strcmp("12345", builder.c_str()), 0);
		GTL_TEST_VERIFY(tc, !builder.overflow());

		builder.append("12345");
		GTL_TEST_EQ(tc, strcmp("1234512345", builder.c_str()), 0);

		builder.append(builder.c_str());
		GTL_TEST_EQ(tc, strcmp("1234512345123451234", builder.c_str()), 0);
		GTL_TEST_VERIFY(tc, builder.overflow());
	}
};

void test_string(Test_Platform& platform)
{
	Test_Suite suite("string", platform);

	Test_Char_Range test_range;
	suite.run("range", test_range);

	Test_Builder test_builder;
	suite.run("builder", test_builder);
}

} //ns
