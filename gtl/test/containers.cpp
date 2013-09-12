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
#include <gtl/containers/ilist.h>
#include <gtl/containers/list.h>
#include <gtl/containers/vector.h>
#include <gtl/containers/registry.h>
#include <gtl/containers/construct.h>
#include <gtl/containers/emplace.h>

using namespace gtl;

namespace gtl {

struct test_thrower
{
	test_thrower()
	{
#if GTL_EXCEPTION
		if(count == 5)
		{
			throw(Exception());
		}
		else
#endif
		{
			++count;
		}
	}

	~test_thrower()
	{
		--count;
	}

	static unsigned int count;
};

unsigned int test_thrower::count = 0;

class Test_Construct : public Gtl_Test_Case
{
public:
	virtual void run(Test_Context& tc)
	{
		int* i = 0;
		construct(i); //should do nothing
		destruct(i);

		char* mem = new char[sizeof(test_thrower) * 10];
		test_thrower* test_throwers = reinterpret_cast<test_thrower*>(mem);
		GTL_TRY
		{
			auto range = make_range(test_throwers, test_throwers + 10);
			construct_range(range);
			destruct_range(range);
		}
		GTL_CATCH_SUPPRESS

			delete[] mem;

		GTL_TEST_EQ(tc, test_thrower::count, 0u);
	}
};

struct emplace_only
{
	emplace_only(int i) : m_i(i)
	{
	}

	int m_i;

private:
	emplace_only(emplace_only const&);
};

class Test_Vector : public Gtl_Test_Case
{
public:
	virtual void run(Test_Context& tc)
	{
		test_thrower::count = 0;

		GTL_TRY
		{
			Vector<test_thrower> array(&m_context, 10);
		}
		GTL_CATCH_SUPPRESS

		GTL_TEST_EQ(tc, m_alloc.outstanding(), 0);
		GTL_TEST_EQ(tc, test_thrower::count, 0u);

		{
			typedef Vector<emplace_only> array_type;
			array_type array(&m_context, 5, Empty_Storage_Tag());
			for(uint i = 0; i < 5; ++i)
			{
				array.fixed_func_emplace_back(emplace(i));
			}

			array_type::range range(array.all());
			array_type::const_range crange(range);

			for(uint i = 0; i < 5; ++i)
			{
				GTL_TEST_EQ(tc, crange.get(i).m_i, i);
			}
		}

		{
			typedef Vector<int> array_type;
			array_type array(&m_context);
			for(int i = 0; i < 5; ++i)
			{
				array.func_emplace_back(emplace());
				array.pop_back();
				array.push_back(i);
			}

			for(int i = 0; i < 5; ++i)
			{
				GTL_TEST_EQ(tc, array[i], i);
			}

			array.clear();
			for(int i = 0; i < 5; ++i)
			{
				array.insert(array.begin(), i);
			}

			for(int i = 0; i < 5; ++i)
			{
				GTL_TEST_EQ(tc, array[i], 4 - i);
			}

			{
				array_type array2(&m_context);
				array2.insert(array2.begin(), array.begin(), array.end());

				for(int i = 0; i < 5; ++i)
				{
					GTL_TEST_EQ(tc, array[i], array2[i]);
				}

				//Another insert after the first element
				array2.reserve(15);
				array2.insert(array2.begin() + 1, array.begin(), array.end());

				for(int i = 0; i < 5; ++i)
				{
					GTL_TEST_EQ(tc, array[i], array2[i + 1]);
				}

				array_type array3(&m_context);
				array3 = array;

				for(int i = 0; i < 5; ++i)
				{
					GTL_TEST_EQ(tc, array[i], array3[i]);
				}

				array3.resize(0);
				array3.resize(10);
			}
		}

		{
			typedef Vector<int> array_type;
			array_type vec(&m_context);

			for(size_t test_size = 4; test_size < 6; ++test_size)
			{
				vec.resize(test_size);
				int ref[5] = {1, 2, 3, 4, 5};
				vec.assign(ref, ref + 5);
				for(int i = 0; i < 5; ++i)
				{
					GTL_TEST_EQ(tc, vec[i], ref[i]);
				}
			}
		}
	}
};

class Test_List_Base : public Gtl_Test_Case
{
public:
	template <class List_T, class Node_T>
	void test_list_base(Test_Context& tc, List_T& list, List_T& temp)
	{
		Node_T n0(0);
		Node_T n1(1);

		typedef List_T list_type;
		GTL_TEST_VERIFY(tc, list.empty());
		GTL_TEST_VERIFY(tc, list.all().empty());

		list.push_front(n0);
		list.push_back(n1);
		GTL_TEST_EQ(tc, list.size(), 2u);

		GTL_TEST_EQ(tc, list.front().i, n0.i);
		GTL_TEST_EQ(tc, list.back().i, n1.i);

		{
			int j = 0;
			for(typename list_type::iterator i = list.begin(); i != list.end(); ++i)
			{
				GTL_TEST_EQ(tc, i->i, j);
				++j;
			}
		}

		{
			int j = 0;
			for(typename list_type::range r = list.all(); !r.empty(); r.pop())
			{
				//Test const range conversion
				typename list_type::const_range r2 = r;
				//Should not compile
				//list_type::range r3 = r2; 
				GTL_TEST_EQ(tc, r2.get().i, j);
				++j;
			}
		}

		{
			size_t size = list.size();
			int j = 0;
			for(typename list_type::reverse_range r = list.reverse(); !r.empty(); r.pop())
			{
				GTL_TEST_EQ(tc, r.get().i, size - j - 1);
				++j;
			}
		}

		list.erase(list.begin(), list.end());
		GTL_TEST_VERIFY(tc, list.empty());

		{
			list.push_back(n0);
			temp.splice(temp.end(), list);
			GTL_TEST_EQ(tc, temp.front().i, 0);

			temp.clear();
			GTL_TEST_VERIFY(tc, list.empty());
		}

		{
			list.push_back(n0);
			list.push_back(n1);

			temp.splice(temp.end(), list, list.begin());
			GTL_TEST_EQ(tc, temp.front().i, 0);
			GTL_TEST_EQ(tc, list.front().i, 1);

			list.clear();
			temp.clear();
			GTL_TEST_VERIFY(tc, list.empty() && temp.empty());
		}
	}
};

class Test_Ilist : public Test_List_Base
{
public:
	struct Tag0 {};
	struct Tag1 {};

	struct Node : public Ilist_Node_T<Tag0>, public Ilist_Node_T<Tag1>
	{
		Node(int i) : i(i) {}

		int i;
	};

	template <class List_T>
	void test_list(Test_Context& tc, List_T& list, List_T& temp)
	{
		test_list_base<List_T, Node>(tc, list, temp);
	}

	virtual void run(Test_Context& tc)
	{
		typedef Ilist<Node, Tag0> ilist_type0;
		typedef Ilist<Node, Tag1> ilist_type1;

		ilist_type0 list;
		ilist_type0 temp;

		test_list(tc, list, temp);

		{
			//inode sharing check
			ilist_type0 list0;
			ilist_type1 list1;

			Node n0(0);
			Node n1(1);

			list0.push_front(n0);
			list0.push_front(n1);
			list1.insert_range(list1.begin(), list0.all());
			GTL_TEST_EQ(tc, &list0.back(), &list1.back());
		}
	};

};

class Test_List : public Test_List_Base
{
public:
	struct Node
	{
		Node(int i) : i(i) {}
		Node(Node const& other) : i(other.i) {}
		int i;
	};

	Test_List() {}

	template <class List_T>
	void test_list(Test_Context& tc, List_T& list, List_T& temp)
	{
		test_list_base<List_T, Node>(tc, list, temp);
	}

	virtual void run(Test_Context& tc)
	{
		typedef List<Node> list_type;

		list_type list(&m_context);
		list_type temp(&m_context);
		test_list(tc, list, temp);

		{
			Node n0(0);
			Node n1(1);

			list.push_back(n0);
			list.push_back(n1);

			temp.insert_range(temp.begin(), list.all());
			GTL_TEST_EQ(tc, list.back().i, temp.back().i);

			list.clear();
			temp.clear();
			GTL_TEST_VERIFY(tc, list.empty() && temp.empty());
		}

	};
};

class Test_Registry : public Gtl_Test_Case
{
public:
	virtual void run(Test_Context& tc)
	{ 
		using namespace registry;
		typedef Node<int> RegNode;

		RegNode top(&m_context);

		uint path[] = {2, 5};
		auto path_range(make_range(path));

		RegNode const* read_node = top.try_get_path(path_range);
		GTL_TEST_VERIFY(tc, !read_node);

		RegNode* write_node = top.get_path(path_range);
		GTL_TEST_VERIFY(tc, write_node != nullptr);

		write_node->data() = 5;
		Fixed_Path<uint, 5> path2;
		path2.append(2).append(5);
		read_node = top.get_path(path2.get());
		GTL_TEST_VERIFY(tc, read_node != nullptr);
		GTL_TEST_EQ(tc, read_node->data(), 5);
		
		path[1] = 3;
		read_node = top.try_get_path(path_range);
		GTL_TEST_VERIFY(tc, !read_node);
	}
};


void test_containers(Test_Platform& platform)
{
	Test_Suite suite("containers", platform);

	Test_Construct test_construct;
	suite.run("construct", test_construct);

	Test_Vector test_vector;
	suite.run("vector", test_vector);

	Test_Ilist test_ilist;
	suite.run("intrusive list", test_ilist);

	Test_List test_list;
	suite.run("list", test_list);

	Test_Registry test_registry;
	suite.run("registry", test_list);
}

} //ns
