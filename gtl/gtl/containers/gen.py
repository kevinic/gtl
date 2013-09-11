import string

def gen_template_type_decls(n):
	decls = []
	for i in range(0, n):
		decls.append("class T{0}".format(i))
	return ", ".join(decls)

def gen_template_types(n):
	decls = []
	for i in range(0, n):
		decls.append("T{0}".format(i))
	return ", ".join(decls)

def gen_ref_parameter_decls(n):
	params = []
	for i in range(0, n):
		params.append("T{0}& p{0}".format(i))
	return ", ".join(params)

def gen_rref_parameter_decls(n):
	params = []
	for i in range(0, n):
		params.append("T{0}&& p{0}".format(i))
	return ", ".join(params)

def gen_parameters(n):
	params = []
	for i in range(0, n):
		params.append("p{0}".format(i))
	return ", ".join(params)

def gen_forward_parameters(n):
	params = []
	for i in range(0, n):
		params.append("std::forward<T{0}>(p{0})".format(i))
	return ", ".join(params)

def gen_initializers(n):
	inits = []
	for i in range(0, n):
		inits.append("p{0}(p{0})".format(i))
	return ", ".join(inits)

def gen_copy_initializers(n, other):
	inits = []
	for i in range(0, n):
		inits.append("p{0}(other.p{0})".format(i))
	return ", ".join(inits)

def gen_member_variables(n):
	inits = []
	for i in range(0, n):
		inits.append("T{0}& p{0};".format(i))
	return "\n\t".join(inits)

def generate(n):
	values = {
		"n" : n,
		"template_type_decls" : gen_template_type_decls(n),
		"ref_parameter_decls" : gen_ref_parameter_decls(n),
		"rref_parameter_decls" : gen_rref_parameter_decls(n),
		"initializers" : gen_initializers(n),
		"copy_initializers_other" : gen_copy_initializers(n, "other"),
		"parameters" : gen_parameters(n),
		"forward_parameters" : gen_forward_parameters(n),
		"member_variables" : gen_member_variables(n),
		"template_types" : gen_template_types(n),
	}

	text = string.Template("""
template <${template_type_decls}>
struct emplace_helper${n}
{
	emplace_helper${n}(${ref_parameter_decls}) :
		${initializers}
	{
	}

	emplace_helper${n}(emplace_helper${n} const& other) :
		${copy_initializers_other}
	{
	}

	template<class T> void operator()(T* p)
	{
		new (p) T(${forward_parameters});
	}

	//Member variables are stored as straight lvalue references,
	//we'll convert them back to proper rvalue refs on the actual invoke
	${member_variables}

private:
	emplace_helper${n}& operator=(emplace_helper${n} const&);
};

template <${template_type_decls}>
inline emplace_helper${n}<${template_types}>
emplace(${rref_parameter_decls})
{
	//Rvalue-ref type captured in the template parameters to the helper
	return emplace_helper${n}<${template_types}>(
		${parameters}
	);
}
""")
	return text.substitute(values)

output = """
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
"""

for i in range(1, 10):
	output += generate(i)


output += """
} //namespace

#endif

"""

open("emplace.h", "wt").write(output)
