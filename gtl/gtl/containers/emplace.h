
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

#ifndef GTL_CONTAINERS_EMPLACE_H
#define GTL_CONTAINERS_EMPLACE_H

#include <gtl/common.h>
#include <utility>
#include "construct.h"

namespace gtl {

struct emplace_helper0
{
	template<class T> void operator()(T* p)
	{
		construct(p);
	}
};

inline emplace_helper0 emplace()
{
	return emplace_helper0();
}

template <class T0>
struct emplace_helper1
{
	emplace_helper1(T0& p0) :
		p0(p0)
	{
	}

	emplace_helper1(emplace_helper1 const& other) :
		p0(other.p0)
	{
	}

	template<class T> void operator()(T* p)
	{
		new (p) T(std::forward<T0>(p0));
	}

	//Member variables are stored as straight lvalue references,
	//we'll convert them back to proper rvalue refs on the actual invoke
	T0& p0;

private:
	emplace_helper1& operator=(emplace_helper1 const&);
};

template <class T0>
inline emplace_helper1<T0>
emplace(T0&& p0)
{
	//Rvalue-ref type captured in the template parameters to the helper
	return emplace_helper1<T0>(
		p0
	);
}

template <class T0, class T1>
struct emplace_helper2
{
	emplace_helper2(T0& p0, T1& p1) :
		p0(p0), p1(p1)
	{
	}

	emplace_helper2(emplace_helper2 const& other) :
		p0(other.p0), p1(other.p1)
	{
	}

	template<class T> void operator()(T* p)
	{
		new (p) T(std::forward<T0>(p0), std::forward<T1>(p1));
	}

	//Member variables are stored as straight lvalue references,
	//we'll convert them back to proper rvalue refs on the actual invoke
	T0& p0;
	T1& p1;

private:
	emplace_helper2& operator=(emplace_helper2 const&);
};

template <class T0, class T1>
inline emplace_helper2<T0, T1>
emplace(T0&& p0, T1&& p1)
{
	//Rvalue-ref type captured in the template parameters to the helper
	return emplace_helper2<T0, T1>(
		p0, p1
	);
}

template <class T0, class T1, class T2>
struct emplace_helper3
{
	emplace_helper3(T0& p0, T1& p1, T2& p2) :
		p0(p0), p1(p1), p2(p2)
	{
	}

	emplace_helper3(emplace_helper3 const& other) :
		p0(other.p0), p1(other.p1), p2(other.p2)
	{
	}

	template<class T> void operator()(T* p)
	{
		new (p) T(std::forward<T0>(p0), std::forward<T1>(p1), std::forward<T2>(p2));
	}

	//Member variables are stored as straight lvalue references,
	//we'll convert them back to proper rvalue refs on the actual invoke
	T0& p0;
	T1& p1;
	T2& p2;

private:
	emplace_helper3& operator=(emplace_helper3 const&);
};

template <class T0, class T1, class T2>
inline emplace_helper3<T0, T1, T2>
emplace(T0&& p0, T1&& p1, T2&& p2)
{
	//Rvalue-ref type captured in the template parameters to the helper
	return emplace_helper3<T0, T1, T2>(
		p0, p1, p2
	);
}

template <class T0, class T1, class T2, class T3>
struct emplace_helper4
{
	emplace_helper4(T0& p0, T1& p1, T2& p2, T3& p3) :
		p0(p0), p1(p1), p2(p2), p3(p3)
	{
	}

	emplace_helper4(emplace_helper4 const& other) :
		p0(other.p0), p1(other.p1), p2(other.p2), p3(other.p3)
	{
	}

	template<class T> void operator()(T* p)
	{
		new (p) T(std::forward<T0>(p0), std::forward<T1>(p1), std::forward<T2>(p2), std::forward<T3>(p3));
	}

	//Member variables are stored as straight lvalue references,
	//we'll convert them back to proper rvalue refs on the actual invoke
	T0& p0;
	T1& p1;
	T2& p2;
	T3& p3;

private:
	emplace_helper4& operator=(emplace_helper4 const&);
};

template <class T0, class T1, class T2, class T3>
inline emplace_helper4<T0, T1, T2, T3>
emplace(T0&& p0, T1&& p1, T2&& p2, T3&& p3)
{
	//Rvalue-ref type captured in the template parameters to the helper
	return emplace_helper4<T0, T1, T2, T3>(
		p0, p1, p2, p3
	);
}

template <class T0, class T1, class T2, class T3, class T4>
struct emplace_helper5
{
	emplace_helper5(T0& p0, T1& p1, T2& p2, T3& p3, T4& p4) :
		p0(p0), p1(p1), p2(p2), p3(p3), p4(p4)
	{
	}

	emplace_helper5(emplace_helper5 const& other) :
		p0(other.p0), p1(other.p1), p2(other.p2), p3(other.p3), p4(other.p4)
	{
	}

	template<class T> void operator()(T* p)
	{
		new (p) T(std::forward<T0>(p0), std::forward<T1>(p1), std::forward<T2>(p2), std::forward<T3>(p3), std::forward<T4>(p4));
	}

	//Member variables are stored as straight lvalue references,
	//we'll convert them back to proper rvalue refs on the actual invoke
	T0& p0;
	T1& p1;
	T2& p2;
	T3& p3;
	T4& p4;

private:
	emplace_helper5& operator=(emplace_helper5 const&);
};

template <class T0, class T1, class T2, class T3, class T4>
inline emplace_helper5<T0, T1, T2, T3, T4>
emplace(T0&& p0, T1&& p1, T2&& p2, T3&& p3, T4&& p4)
{
	//Rvalue-ref type captured in the template parameters to the helper
	return emplace_helper5<T0, T1, T2, T3, T4>(
		p0, p1, p2, p3, p4
	);
}

template <class T0, class T1, class T2, class T3, class T4, class T5>
struct emplace_helper6
{
	emplace_helper6(T0& p0, T1& p1, T2& p2, T3& p3, T4& p4, T5& p5) :
		p0(p0), p1(p1), p2(p2), p3(p3), p4(p4), p5(p5)
	{
	}

	emplace_helper6(emplace_helper6 const& other) :
		p0(other.p0), p1(other.p1), p2(other.p2), p3(other.p3), p4(other.p4), p5(other.p5)
	{
	}

	template<class T> void operator()(T* p)
	{
		new (p) T(std::forward<T0>(p0), std::forward<T1>(p1), std::forward<T2>(p2), std::forward<T3>(p3), std::forward<T4>(p4), std::forward<T5>(p5));
	}

	//Member variables are stored as straight lvalue references,
	//we'll convert them back to proper rvalue refs on the actual invoke
	T0& p0;
	T1& p1;
	T2& p2;
	T3& p3;
	T4& p4;
	T5& p5;

private:
	emplace_helper6& operator=(emplace_helper6 const&);
};

template <class T0, class T1, class T2, class T3, class T4, class T5>
inline emplace_helper6<T0, T1, T2, T3, T4, T5>
emplace(T0&& p0, T1&& p1, T2&& p2, T3&& p3, T4&& p4, T5&& p5)
{
	//Rvalue-ref type captured in the template parameters to the helper
	return emplace_helper6<T0, T1, T2, T3, T4, T5>(
		p0, p1, p2, p3, p4, p5
	);
}

template <class T0, class T1, class T2, class T3, class T4, class T5, class T6>
struct emplace_helper7
{
	emplace_helper7(T0& p0, T1& p1, T2& p2, T3& p3, T4& p4, T5& p5, T6& p6) :
		p0(p0), p1(p1), p2(p2), p3(p3), p4(p4), p5(p5), p6(p6)
	{
	}

	emplace_helper7(emplace_helper7 const& other) :
		p0(other.p0), p1(other.p1), p2(other.p2), p3(other.p3), p4(other.p4), p5(other.p5), p6(other.p6)
	{
	}

	template<class T> void operator()(T* p)
	{
		new (p) T(std::forward<T0>(p0), std::forward<T1>(p1), std::forward<T2>(p2), std::forward<T3>(p3), std::forward<T4>(p4), std::forward<T5>(p5), std::forward<T6>(p6));
	}

	//Member variables are stored as straight lvalue references,
	//we'll convert them back to proper rvalue refs on the actual invoke
	T0& p0;
	T1& p1;
	T2& p2;
	T3& p3;
	T4& p4;
	T5& p5;
	T6& p6;

private:
	emplace_helper7& operator=(emplace_helper7 const&);
};

template <class T0, class T1, class T2, class T3, class T4, class T5, class T6>
inline emplace_helper7<T0, T1, T2, T3, T4, T5, T6>
emplace(T0&& p0, T1&& p1, T2&& p2, T3&& p3, T4&& p4, T5&& p5, T6&& p6)
{
	//Rvalue-ref type captured in the template parameters to the helper
	return emplace_helper7<T0, T1, T2, T3, T4, T5, T6>(
		p0, p1, p2, p3, p4, p5, p6
	);
}

template <class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7>
struct emplace_helper8
{
	emplace_helper8(T0& p0, T1& p1, T2& p2, T3& p3, T4& p4, T5& p5, T6& p6, T7& p7) :
		p0(p0), p1(p1), p2(p2), p3(p3), p4(p4), p5(p5), p6(p6), p7(p7)
	{
	}

	emplace_helper8(emplace_helper8 const& other) :
		p0(other.p0), p1(other.p1), p2(other.p2), p3(other.p3), p4(other.p4), p5(other.p5), p6(other.p6), p7(other.p7)
	{
	}

	template<class T> void operator()(T* p)
	{
		new (p) T(std::forward<T0>(p0), std::forward<T1>(p1), std::forward<T2>(p2), std::forward<T3>(p3), std::forward<T4>(p4), std::forward<T5>(p5), std::forward<T6>(p6), std::forward<T7>(p7));
	}

	//Member variables are stored as straight lvalue references,
	//we'll convert them back to proper rvalue refs on the actual invoke
	T0& p0;
	T1& p1;
	T2& p2;
	T3& p3;
	T4& p4;
	T5& p5;
	T6& p6;
	T7& p7;

private:
	emplace_helper8& operator=(emplace_helper8 const&);
};

template <class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7>
inline emplace_helper8<T0, T1, T2, T3, T4, T5, T6, T7>
emplace(T0&& p0, T1&& p1, T2&& p2, T3&& p3, T4&& p4, T5&& p5, T6&& p6, T7&& p7)
{
	//Rvalue-ref type captured in the template parameters to the helper
	return emplace_helper8<T0, T1, T2, T3, T4, T5, T6, T7>(
		p0, p1, p2, p3, p4, p5, p6, p7
	);
}

template <class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
struct emplace_helper9
{
	emplace_helper9(T0& p0, T1& p1, T2& p2, T3& p3, T4& p4, T5& p5, T6& p6, T7& p7, T8& p8) :
		p0(p0), p1(p1), p2(p2), p3(p3), p4(p4), p5(p5), p6(p6), p7(p7), p8(p8)
	{
	}

	emplace_helper9(emplace_helper9 const& other) :
		p0(other.p0), p1(other.p1), p2(other.p2), p3(other.p3), p4(other.p4), p5(other.p5), p6(other.p6), p7(other.p7), p8(other.p8)
	{
	}

	template<class T> void operator()(T* p)
	{
		new (p) T(std::forward<T0>(p0), std::forward<T1>(p1), std::forward<T2>(p2), std::forward<T3>(p3), std::forward<T4>(p4), std::forward<T5>(p5), std::forward<T6>(p6), std::forward<T7>(p7), std::forward<T8>(p8));
	}

	//Member variables are stored as straight lvalue references,
	//we'll convert them back to proper rvalue refs on the actual invoke
	T0& p0;
	T1& p1;
	T2& p2;
	T3& p3;
	T4& p4;
	T5& p5;
	T6& p6;
	T7& p7;
	T8& p8;

private:
	emplace_helper9& operator=(emplace_helper9 const&);
};

template <class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
inline emplace_helper9<T0, T1, T2, T3, T4, T5, T6, T7, T8>
emplace(T0&& p0, T1&& p1, T2&& p2, T3&& p3, T4&& p4, T5&& p5, T6&& p6, T7&& p7, T8&& p8)
{
	//Rvalue-ref type captured in the template parameters to the helper
	return emplace_helper9<T0, T1, T2, T3, T4, T5, T6, T7, T8>(
		p0, p1, p2, p3, p4, p5, p6, p7, p8
	);
}

} //namespace

#endif

