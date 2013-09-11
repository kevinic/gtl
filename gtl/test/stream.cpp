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
#include <gtl/stream.h>
#include <gtl/containers/vector.h>

namespace gtl {

class Test_Stream : public Gtl_Test_Case
{
public:
	virtual void run(Test_Context& tc)
	{
		int ref[4] = {0, 1, 3, 4};
		{
			int out[4] = {0};

			auto in_stream = range_in_stream<int>(make_range(ref));
			auto out_stream = range_out_stream<int>(make_range(out));

			while(!in_stream.eof() && !out_stream.eof())
			{
				out_stream.put(in_stream.get());
			}

			for(uint i = 0; i < 4; ++i)
			{
				GTL_TEST_EQ(tc, ref[i], out[i]);
			}
		}

		{
			gtl::Vector<int> in(&m_context);
			gtl::Vector<int> out(&m_context);

			in.assign(ref, ref + 4);
			auto in_stream = container_in_stream<int>(in);
			auto out_stream = container_out_stream<int>(out);

			while(!in_stream.eof() && !out_stream.eof())
			{
				out_stream.put(in_stream.get());
			}

			for(uint i = 0; i < 4; ++i)
			{
				GTL_TEST_EQ(tc, ref[i], out[i]);
			}
		}

		{
			gtl::Vector<int> in(&m_context);
			gtl::Vector<int> out(&m_context);

			in.assign(ref, ref + 4);
			auto in_stream = container_in_stream<int>(in);
			auto in_adapter = adapter_in_stream<int>(in_stream, [](int x) -> int {return x + 1;});

			auto out_stream = container_out_stream<int>(out);
			auto out_adapter = adapter_out_stream<int>(out_stream, [](int x) -> int {return x + 2;});

			while(!in_adapter.eof() && !out_adapter.eof())
			{
				out_adapter.put(in_adapter.get());
			}

			for(uint i = 0; i < 4; ++i)
			{
				GTL_TEST_EQ(tc, ref[i] + 3, out[i]);
			}

		}
	}
};

void test_stream(Test_Platform& platform)
{
	Test_Suite suite("stream", platform);

	Test_Stream test_stream;
	suite.run("stream", test_stream);
}

} //ns
