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

#ifndef GTL_TEST_H
#define GTL_TEST_H

#include "common.h"
#include <gtl/noncopyable.h>
#include <gtl/debug.h>
#include <gtl/diagnostics/exception.h>
#include <gtl/string.h>

namespace gtl {

class Test_Context;

class Test_Case
{
public:
	virtual void run(Test_Context& context) = 0;

protected:
	virtual ~Test_Case() {}
};

class Test_Platform
{
public:
	virtual void output(char const* string) = 0;

protected:
	virtual ~Test_Platform() {}
};

struct Test_Exception : public Exception
{
	Test_Exception(char const* text) : Exception(text) {}
};

class Test_Context : private Noncopyable
{
public:
	Test_Context(char const* name, Test_Platform& platform) :
		m_name(name),
		m_platform(platform) {}
	
	void verify(bool result, char const* file, unsigned int line)
	{
		GTL_ASSERT(result);

		if(!result)
		{
			fail(file, line);
		}
	}

	template <class T, class TT>
	void verify_eq(T const& actual, TT const& expected, char const* file, unsigned int line)
	{
		//Give the debugger a chance to catch the actual value, maybe do
		//formatted output in the future
		GTL_ASSERT(actual == expected);

		if(actual != expected)
		{
			fail(file, line);
		}
	}

private:
	void fail(char const* file, unsigned int line)
	{
		char buffer[300];
		string::snprintf(buffer, 300, "Test %s failed at %s(%u)", m_name, file, line);
		m_platform.output(buffer);

		raise_exception(Test_Exception(buffer));

	}

private:
	char const* m_name;
	Test_Platform& m_platform;
};

#define GTL_TEST_VERIFY(tc, result) tc.verify(result, __FILE__, __LINE__)
#define GTL_TEST_EQ(tc, actual, expected) tc.verify_eq(actual, expected, __FILE__, __LINE__)

class Test_Suite : private Noncopyable
{
public:
	Test_Suite(char const* name, Test_Platform& platform) :
		m_name(name),
		m_platform(platform),
		m_result(true),
		m_passed(0),
		m_total(0)
	{
		char buffer[300];
		string::snprintf(buffer, 300, "Executing suite %s", name);
		m_platform.output(buffer);
	}

	~Test_Suite()
	{
		char buffer[300];

		string::snprintf(buffer, 300,
			"Completed suite %s, passed %u of %u: %s", m_name, m_passed, m_total, m_result ? "success" : "FAILED");

		m_platform.output(buffer);
	}

	void run(char const* name, Test_Case& test_case)
	{
		Test_Context context(name, m_platform);
		bool passed = true;
#if GTL_EXCEPTION
		try
		{
			test_case.run(context);
		}
		catch(Test_Exception const&)
		{
			m_result = false;
			passed = false;
		}
#else
		test_case.run(context);
#endif
		if(passed)
		{
			m_passed++;
		}
		m_total++;
	}

	bool result() const
	{
		return m_result;
	}

private:
	char const* m_name;
	Test_Platform& m_platform;
	bool m_result;
	unsigned int m_passed;
	unsigned int m_total;
};

} //namespace

#endif

