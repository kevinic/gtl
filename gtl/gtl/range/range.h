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

#ifndef GTL_RANGE_RANGE_H
#define GTL_RANGE_RANGE_H

#include <gtl/common.h>
#include <gtl/type_traits.h>

namespace gtl {

//For now assume all ranges are at least forward (multi pass), so we don't
//have to deal with the distinction of input and forward ranges
template <class T> struct Range
{
	typedef T& ref_type;
	typedef T const& get_type;
	typedef T const& set_type;

	Range(Range const& other);

	ref_type ref() const;
	get_type get() const;
	void set(set_type type) const;
	void pop();
	
	bool empty() const;

	//random
	get_type get(size_t i) const;
	void set(size_t i, set_type) const;
	Range slice(size_t begin, size_t end);
	size_t size() const;

	//provided for range based for loop support only, return an iterator that may be deref'd
	void* begin() const;
	void* end() const;
};

template <class Range_T> struct is_random_range
{
	typedef false_type type;
};

} //ns

#endif
