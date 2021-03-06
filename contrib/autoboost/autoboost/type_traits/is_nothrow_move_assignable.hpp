
//  (C) Copyright Steve Cleary, Beman Dawes, Howard Hinnant & John Maddock 2000.
//  (C) Copyright Eric Friedman 2002-2003.
//  (C) Copyright Antony Polukhin 2013.
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.

#ifndef AUTOBOOST_TT_IS_NOTHROW_MOVE_ASSIGNABLE_HPP_INCLUDED
#define AUTOBOOST_TT_IS_NOTHROW_MOVE_ASSIGNABLE_HPP_INCLUDED

#include <autoboost/config.hpp>
#include <autoboost/type_traits/has_trivial_move_assign.hpp>
#include <autoboost/type_traits/has_nothrow_assign.hpp>
#include <autoboost/type_traits/is_array.hpp>
#include <autoboost/type_traits/is_reference.hpp>
#include <autoboost/type_traits/detail/ice_and.hpp>
#include <autoboost/type_traits/detail/ice_or.hpp>
#include <autoboost/type_traits/detail/ice_not.hpp>
#include <autoboost/utility/enable_if.hpp>
#include <autoboost/utility/declval.hpp>

// should be the last #include
#include <autoboost/type_traits/detail/bool_trait_def.hpp>

namespace autoboost {

namespace detail{

#if !defined(AUTOBOOST_NO_CXX11_NOEXCEPT) && !defined(AUTOBOOST_NO_SFINAE_EXPR)

template <class T, class Enable = void>
struct false_or_cpp11_noexcept_move_assignable: public ::autoboost::false_type {};

template <class T>
struct false_or_cpp11_noexcept_move_assignable <
        T,
        typename ::autoboost::enable_if_c<sizeof(T) && AUTOBOOST_NOEXCEPT_EXPR(::autoboost::declval<T&>() = ::autoboost::declval<T>())>::type
    > : public ::autoboost::integral_constant<bool, AUTOBOOST_NOEXCEPT_EXPR(::autoboost::declval<T&>() = ::autoboost::declval<T>())>
{};

template <class T>
struct is_nothrow_move_assignable_imp{
    AUTOBOOST_STATIC_CONSTANT(bool, value = ::autoboost::detail::false_or_cpp11_noexcept_move_assignable<T>::value);
};

template <class T>
struct is_nothrow_move_assignable_imp<T const> : public ::autoboost::false_type {};
template <class T>
struct is_nothrow_move_assignable_imp<T volatile> : public ::autoboost::false_type{};
template <class T>
struct is_nothrow_move_assignable_imp<T const volatile> : public ::autoboost::false_type{};
template <class T>
struct is_nothrow_move_assignable_imp<T&> : public ::autoboost::false_type{};
#ifndef AUTOBOOST_NO_CXX11_RVALUE_REFERENCES
template <class T>
struct is_nothrow_move_assignable_imp<T&&> : public ::autoboost::false_type{};
#endif

#else

template <class T>
struct is_nothrow_move_assignable_imp{
    AUTOBOOST_STATIC_CONSTANT(bool, value = (
        ::autoboost::type_traits::ice_and<
            ::autoboost::type_traits::ice_or<
                ::autoboost::has_trivial_move_assign<T>::value,
                ::autoboost::has_nothrow_assign<T>::value
            >::value,
            ::autoboost::type_traits::ice_not< ::autoboost::is_array<T>::value >::value
        >::value));
};

#endif

}

AUTOBOOST_TT_AUX_BOOL_TRAIT_DEF1(is_nothrow_move_assignable,T,::autoboost::detail::is_nothrow_move_assignable_imp<T>::value)
AUTOBOOST_TT_AUX_BOOL_TRAIT_SPEC1(is_nothrow_move_assignable,void,false)
#ifndef AUTOBOOST_NO_CV_VOID_SPECIALIZATIONS
AUTOBOOST_TT_AUX_BOOL_TRAIT_SPEC1(is_nothrow_move_assignable,void const,false)
AUTOBOOST_TT_AUX_BOOL_TRAIT_SPEC1(is_nothrow_move_assignable,void const volatile,false)
AUTOBOOST_TT_AUX_BOOL_TRAIT_SPEC1(is_nothrow_move_assignable,void volatile,false)
#endif

} // namespace autoboost

#include <autoboost/type_traits/detail/bool_trait_undef.hpp>

#endif // AUTOBOOST_TT_IS_NOTHROW_MOVE_ASSIGNABLE_HPP_INCLUDED
