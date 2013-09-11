===============
Overview
===============

GTL is:

* a simple C++ template library that's aimed towards game development
* (mostly) header based
* focused on sensible C++ practices.  To that end, the code does not shy away
  from C++11 syntax such as auto and lambdas.

===============
Usage
===============

For now, only the VS2012 solution is provided.  However, GTL only contains
a couple of non-header files:

* cstr.cpp - shileds C headers from inclusion
* debug_impl.cpp - platform dependent implementation of debug functions (assert, breakpoint, & etc).

Stub or replace them as necessary on other platforms.

---------------
Allocation
---------------

GTL provides a solidified, generic allocation interface.  All GTL classes that
allocate do so through a "context" pointer that contains a pointer to said
interface.

Allocators do not contaminate container types (like STL), and no allocation is
made without context (no global new/delete/malloc/free).

---------------
Diagnostics
---------------

Provides simple definitions for assert and exception.  Implementations for
the functions are provided in platform-dependent folders, and library users
are free to exclude them in favor of custom implementation.

The library aims to be exception safe, but does not require exceptions to be
enabled.

Simple unit test classes are provided as well.

--------------
Containers
--------------

The library is NOT meant to be a replacement for STL.  However, list (including
intrusive list) and vector container implementations are provided. They follow
the allocation model laid out above.

Moreover, range definitions are provided in addition to iterators, making
certain lazy evaluations possible (such as range splicing, joining, & etc).  It
also makes abstracting enumerators easier.

Instead of move constructors, the containers have insertion methods that simply
take a functor, which can be used to construct raw memory in any fashion.

Associative containers are discouraged and excluded from the library.

--------------
Strings
--------------

No string containers are provided.  The string library in GTL simply works
with a "character range", which is a pair of constant character pointers. This
again enables splicing & etc.

A type safe format is provided with the following syntax:

.. code::

    gtl::format::Expression exp = gtl::fmt("int %i float %f").with(5).with(0.1f);
    gtl::format::snprintf(buffer, sizeof(buffer), exp);

-------------
Scope
-------------

A generic Scoped class is provided to free data on exiting scope.

There is also a simple scope guard implementation that executes a piece of
code on scope exit.

.. code::

    GTL_SCOPE([=](){destroy(m_data);});
