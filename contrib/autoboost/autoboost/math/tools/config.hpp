//  Copyright (c) 2006-7 John Maddock
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef AUTOBOOST_MATH_TOOLS_CONFIG_HPP
#define AUTOBOOST_MATH_TOOLS_CONFIG_HPP

#ifdef _MSC_VER
#pragma once
#endif

#include <autoboost/config.hpp>
#include <autoboost/cstdint.hpp> // for autoboost::uintmax_t
#include <autoboost/detail/workaround.hpp>
#include <autoboost/type_traits/is_integral.hpp>
#include <algorithm>  // for min and max
#include <autoboost/config/no_tr1/cmath.hpp>
#include <climits>
#include <cfloat>
#if (defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__))
#  include <math.h>
#endif
#ifndef AUTOBOOST_NO_LIMITS_COMPILE_TIME_CONSTANTS
#  include <limits>
#endif

#include <autoboost/math/tools/user.hpp>

#if (defined(__CYGWIN__) || defined(__FreeBSD__) || defined(__NetBSD__) \
   || (defined(__hppa) && !defined(__OpenBSD__)) || (defined(__NO_LONG_DOUBLE_MATH) && (DBL_MANT_DIG != LDBL_MANT_DIG))) \
   && !defined(AUTOBOOST_MATH_NO_LONG_DOUBLE_MATH_FUNCTIONS)
#  define AUTOBOOST_MATH_NO_LONG_DOUBLE_MATH_FUNCTIONS
#endif
#if AUTOBOOST_WORKAROUND(__BORLANDC__, AUTOBOOST_TESTED_AT(0x582))
//
// Borland post 5.8.2 uses Dinkumware's std C lib which
// doesn't have true long double precision.  Earlier
// versions are problematic too:
//
#  define AUTOBOOST_MATH_NO_REAL_CONCEPT_TESTS
#  define AUTOBOOST_MATH_NO_LONG_DOUBLE_MATH_FUNCTIONS
#  define AUTOBOOST_MATH_CONTROL_FP _control87(MCW_EM,MCW_EM)
#  include <float.h>
#endif
#ifdef __IBMCPP__
//
// For reasons I don't unserstand, the tests with IMB's compiler all
// pass at long double precision, but fail with real_concept, those tests
// are disabled for now.  (JM 2012).
#  define AUTOBOOST_MATH_NO_REAL_CONCEPT_TESTS
#endif
#if (defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__)) && ((LDBL_MANT_DIG == 106) || (__LDBL_MANT_DIG__ == 106)) && !defined(AUTOBOOST_MATH_NO_LONG_DOUBLE_MATH_FUNCTIONS)
//
// Darwin's rather strange "double double" is rather hard to
// support, it should be possible given enough effort though...
//
#  define AUTOBOOST_MATH_NO_LONG_DOUBLE_MATH_FUNCTIONS
#endif
#if defined(unix) && defined(__INTEL_COMPILER) && (__INTEL_COMPILER <= 1000) && !defined(AUTOBOOST_MATH_NO_LONG_DOUBLE_MATH_FUNCTIONS)
//
// Intel compiler prior to version 10 has sporadic problems
// calling the long double overloads of the std lib math functions:
// calling ::powl is OK, but std::pow(long double, long double)
// may segfault depending upon the value of the arguments passed
// and the specific Linux distribution.
//
// We'll be conservative and disable long double support for this compiler.
//
// Comment out this #define and try building the tests to determine whether
// your Intel compiler version has this issue or not.
//
#  define AUTOBOOST_MATH_NO_LONG_DOUBLE_MATH_FUNCTIONS
#endif
#if defined(unix) && defined(__INTEL_COMPILER)
//
// Intel compiler has sporadic issues compiling std::fpclassify depending on
// the exact OS version used.  Use our own code for this as we know it works
// well on Intel processors:
//
#define AUTOBOOST_MATH_DISABLE_STD_FPCLASSIFY
#endif

#if defined(AUTOBOOST_MSVC) && !defined(_WIN32_WCE)
   // Better safe than sorry, our tests don't support hardware exceptions:
#  define AUTOBOOST_MATH_CONTROL_FP _control87(MCW_EM,MCW_EM)
#endif

#ifdef __IBMCPP__
#  define AUTOBOOST_MATH_NO_DEDUCED_FUNCTION_POINTERS
#endif

#if (defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901))
#  define AUTOBOOST_MATH_USE_C99
#endif

#if (defined(__hpux) && !defined(__hppa))
#  define AUTOBOOST_MATH_USE_C99
#endif

#if defined(__GNUC__) && defined(_GLIBCXX_USE_C99)
#  define AUTOBOOST_MATH_USE_C99
#endif

#if defined(_LIBCPP_VERSION) && !defined(_MSC_VER)
#  define AUTOBOOST_MATH_USE_C99
#endif

#if defined(__CYGWIN__) || defined(__HP_aCC) || defined(AUTOBOOST_INTEL) \
  || defined(AUTOBOOST_NO_NATIVE_LONG_DOUBLE_FP_CLASSIFY) \
  || (defined(__GNUC__) && !defined(AUTOBOOST_MATH_USE_C99))\
  || defined(AUTOBOOST_MATH_NO_LONG_DOUBLE_MATH_FUNCTIONS)
#  define AUTOBOOST_MATH_NO_NATIVE_LONG_DOUBLE_FP_CLASSIFY
#endif

#if AUTOBOOST_WORKAROUND(__SUNPRO_CC, <= 0x590)

#  include "autoboost/type.hpp"
#  include "autoboost/non_type.hpp"

#  define AUTOBOOST_MATH_EXPLICIT_TEMPLATE_TYPE(t)         autoboost::type<t>* = 0
#  define AUTOBOOST_MATH_EXPLICIT_TEMPLATE_TYPE_SPEC(t)    autoboost::type<t>*
#  define AUTOBOOST_MATH_EXPLICIT_TEMPLATE_NON_TYPE(t, v)  autoboost::non_type<t, v>* = 0
#  define AUTOBOOST_MATH_EXPLICIT_TEMPLATE_NON_TYPE_SPEC(t, v)  autoboost::non_type<t, v>*

#  define AUTOBOOST_MATH_APPEND_EXPLICIT_TEMPLATE_TYPE(t)         \
             , AUTOBOOST_MATH_EXPLICIT_TEMPLATE_TYPE(t)
#  define AUTOBOOST_MATH_APPEND_EXPLICIT_TEMPLATE_TYPE_SPEC(t)    \
             , AUTOBOOST_MATH_EXPLICIT_TEMPLATE_TYPE_SPEC(t)
#  define AUTOBOOST_MATH_APPEND_EXPLICIT_TEMPLATE_NON_TYPE(t, v)  \
             , AUTOBOOST_MATH_EXPLICIT_TEMPLATE_NON_TYPE(t, v)
#  define AUTOBOOST_MATH_APPEND_EXPLICIT_TEMPLATE_NON_TYPE_SPEC(t, v)  \
             , AUTOBOOST_MATH_EXPLICIT_TEMPLATE_NON_TYPE_SPEC(t, v)

#else

// no workaround needed: expand to nothing

#  define AUTOBOOST_MATH_EXPLICIT_TEMPLATE_TYPE(t)
#  define AUTOBOOST_MATH_EXPLICIT_TEMPLATE_TYPE_SPEC(t)
#  define AUTOBOOST_MATH_EXPLICIT_TEMPLATE_NON_TYPE(t, v)
#  define AUTOBOOST_MATH_EXPLICIT_TEMPLATE_NON_TYPE_SPEC(t, v)

#  define AUTOBOOST_MATH_APPEND_EXPLICIT_TEMPLATE_TYPE(t)
#  define AUTOBOOST_MATH_APPEND_EXPLICIT_TEMPLATE_TYPE_SPEC(t)
#  define AUTOBOOST_MATH_APPEND_EXPLICIT_TEMPLATE_NON_TYPE(t, v)
#  define AUTOBOOST_MATH_APPEND_EXPLICIT_TEMPLATE_NON_TYPE_SPEC(t, v)


#endif // __SUNPRO_CC

#if (defined(__SUNPRO_CC) || defined(__hppa) || defined(__GNUC__)) && !defined(AUTOBOOST_MATH_SMALL_CONSTANT)
// Sun's compiler emits a hard error if a constant underflows,
// as does aCC on PA-RISC, while gcc issues a large number of warnings:
#  define AUTOBOOST_MATH_SMALL_CONSTANT(x) 0.0
#else
#  define AUTOBOOST_MATH_SMALL_CONSTANT(x) x
#endif


#if AUTOBOOST_WORKAROUND(AUTOBOOST_MSVC, < 1400)
//
// Define if constants too large for a float cause "bad"
// values to be stored in the data, rather than infinity
// or a suitably large value.
//
#  define AUTOBOOST_MATH_BUGGY_LARGE_FLOAT_CONSTANTS
#endif
//
// Tune performance options for specific compilers:
//
#ifdef AUTOBOOST_MSVC
#  define AUTOBOOST_MATH_POLY_METHOD 2
#elif defined(AUTOBOOST_INTEL)
#  define AUTOBOOST_MATH_POLY_METHOD 2
#  define AUTOBOOST_MATH_RATIONAL_METHOD 2
#elif defined(__GNUC__)
#  define AUTOBOOST_MATH_POLY_METHOD 3
#  define AUTOBOOST_MATH_RATIONAL_METHOD 3
#  define AUTOBOOST_MATH_INT_TABLE_TYPE(RT, IT) RT
#  define AUTOBOOST_MATH_INT_VALUE_SUFFIX(RV, SUF) RV##.0L
#endif

#if defined(AUTOBOOST_NO_LONG_LONG) && !defined(AUTOBOOST_MATH_INT_TABLE_TYPE)
#  define AUTOBOOST_MATH_INT_TABLE_TYPE(RT, IT) RT
#  define AUTOBOOST_MATH_INT_VALUE_SUFFIX(RV, SUF) RV##.0L
#endif

//
// The maximum order of polynomial that will be evaluated
// via an unrolled specialisation:
//
#ifndef AUTOBOOST_MATH_MAX_POLY_ORDER
#  define AUTOBOOST_MATH_MAX_POLY_ORDER 17
#endif
//
// Set the method used to evaluate polynomials and rationals:
//
#ifndef AUTOBOOST_MATH_POLY_METHOD
#  define AUTOBOOST_MATH_POLY_METHOD 1
#endif
#ifndef AUTOBOOST_MATH_RATIONAL_METHOD
#  define AUTOBOOST_MATH_RATIONAL_METHOD 0
#endif
//
// decide whether to store constants as integers or reals:
//
#ifndef AUTOBOOST_MATH_INT_TABLE_TYPE
#  define AUTOBOOST_MATH_INT_TABLE_TYPE(RT, IT) IT
#endif
#ifndef AUTOBOOST_MATH_INT_VALUE_SUFFIX
#  define AUTOBOOST_MATH_INT_VALUE_SUFFIX(RV, SUF) RV##SUF
#endif
//
// Test whether to support __float128:
//
#if defined(_GLIBCXX_USE_FLOAT128) && defined(AUTOBOOST_GCC) && !defined(__STRICT_ANSI__) \
   && !defined(AUTOBOOST_MATH_DISABLE_FLOAT128) || defined(AUTOBOOST_MATH_USE_FLOAT128)
//
// Only enable this when the compiler really is GCC as clang and probably
// intel too don't support __float128 yet :-(
//
#ifndef AUTOBOOST_MATH_USE_FLOAT128
#  define AUTOBOOST_MATH_USE_FLOAT128
#endif

#  if defined(AUTOBOOST_INTEL) && defined(AUTOBOOST_INTEL_CXX_VERSION) && (AUTOBOOST_INTEL_CXX_VERSION >= 1310) && defined(__GNUC__)
#    if (__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 6))
#      define AUTOBOOST_MATH_FLOAT128_TYPE __float128
#    endif
#  elif defined(__GNUC__)
#      define AUTOBOOST_MATH_FLOAT128_TYPE __float128
#  endif

#  ifndef AUTOBOOST_MATH_FLOAT128_TYPE
#      define AUTOBOOST_MATH_FLOAT128_TYPE _Quad
#  endif
#endif
//
// Check for WinCE with no iostream support:
//
#if defined(_WIN32_WCE) && !defined(__SGI_STL_PORT)
#  define AUTOBOOST_MATH_NO_LEXICAL_CAST
#endif

//
// Helper macro for controlling the FP behaviour:
//
#ifndef AUTOBOOST_MATH_CONTROL_FP
#  define AUTOBOOST_MATH_CONTROL_FP
#endif
//
// Helper macro for using statements:
//
#define AUTOBOOST_MATH_STD_USING_CORE \
   using std::abs;\
   using std::acos;\
   using std::cos;\
   using std::fmod;\
   using std::modf;\
   using std::tan;\
   using std::asin;\
   using std::cosh;\
   using std::frexp;\
   using std::pow;\
   using std::tanh;\
   using std::atan;\
   using std::exp;\
   using std::ldexp;\
   using std::sin;\
   using std::atan2;\
   using std::fabs;\
   using std::log;\
   using std::sinh;\
   using std::ceil;\
   using std::floor;\
   using std::log10;\
   using std::sqrt;

#define AUTOBOOST_MATH_STD_USING AUTOBOOST_MATH_STD_USING_CORE

namespace autoboost{ namespace math{
namespace tools
{

template <class T>
inline T max AUTOBOOST_PREVENT_MACRO_SUBSTITUTION(T a, T b, T c)
{
   return (std::max)((std::max)(a, b), c);
}

template <class T>
inline T max AUTOBOOST_PREVENT_MACRO_SUBSTITUTION(T a, T b, T c, T d)
{
   return (std::max)((std::max)(a, b), (std::max)(c, d));
}

} // namespace tools

template <class T>
void suppress_unused_variable_warning(const T&)
{
}

namespace detail{

template <class T>
struct is_integer_for_rounding
{
   static const bool value = autoboost::is_integral<T>::value
#ifndef AUTOBOOST_NO_LIMITS_COMPILE_TIME_CONSTANTS
      || (std::numeric_limits<T>::is_specialized && std::numeric_limits<T>::is_integer)
#endif
      ;
};

}

}} // namespace autoboost namespace math

#ifdef __GLIBC_PREREQ
#  if __GLIBC_PREREQ(2,14)
#     define AUTOBOOST_MATH_HAVE_FIXED_GLIBC
#  endif
#endif

#if ((defined(__linux__) && !defined(__UCLIBC__) && !defined(AUTOBOOST_MATH_HAVE_FIXED_GLIBC)) || defined(__QNX__) || defined(__IBMCPP__)) && !defined(AUTOBOOST_NO_FENV_H)
//
// This code was introduced in response to this glibc bug: http://sourceware.org/bugzilla/show_bug.cgi?id=2445
// Basically powl and expl can return garbage when the result is small and certain exception flags are set
// on entrance to these functions.  This appears to have been fixed in Glibc 2.14 (May 2011).
// Much more information in this message thread: https://groups.google.com/forum/#!topic/autoboost-list/ZT99wtIFlb4
//

   #include <autoboost/detail/fenv.hpp>

#  ifdef FE_ALL_EXCEPT

namespace autoboost{ namespace math{
   namespace detail
   {
   struct fpu_guard
   {
      fpu_guard()
      {
         fegetexceptflag(&m_flags, FE_ALL_EXCEPT);
         feclearexcept(FE_ALL_EXCEPT);
      }
      ~fpu_guard()
      {
         fesetexceptflag(&m_flags, FE_ALL_EXCEPT);
      }
   private:
      fexcept_t m_flags;
   };

   } // namespace detail
   }} // namespaces

#    define AUTOBOOST_FPU_EXCEPTION_GUARD autoboost::math::detail::fpu_guard local_guard_object;
#    define AUTOBOOST_MATH_INSTRUMENT_FPU do{ fexcept_t cpu_flags; fegetexceptflag(&cpu_flags, FE_ALL_EXCEPT); AUTOBOOST_MATH_INSTRUMENT_VARIABLE(cpu_flags); } while(0);

#  else

#    define AUTOBOOST_FPU_EXCEPTION_GUARD
#    define AUTOBOOST_MATH_INSTRUMENT_FPU

#  endif

#else // All other platforms.
#  define AUTOBOOST_FPU_EXCEPTION_GUARD
#  define AUTOBOOST_MATH_INSTRUMENT_FPU
#endif

#ifdef AUTOBOOST_MATH_INSTRUMENT

#  include <iostream>
#  include <iomanip>
#  include <typeinfo>

#  define AUTOBOOST_MATH_INSTRUMENT_CODE(x) \
      std::cout << std::setprecision(35) << __FILE__ << ":" << __LINE__ << " " << x << std::endl;
#  define AUTOBOOST_MATH_INSTRUMENT_VARIABLE(name) AUTOBOOST_MATH_INSTRUMENT_CODE(AUTOBOOST_STRINGIZE(name) << " = " << name)

#else

#  define AUTOBOOST_MATH_INSTRUMENT_CODE(x)
#  define AUTOBOOST_MATH_INSTRUMENT_VARIABLE(name)

#endif

#endif // AUTOBOOST_MATH_TOOLS_CONFIG_HPP





