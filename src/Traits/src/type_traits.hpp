/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   type_traits.hpp                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/02 12:12:08 by rlucas        #+#    #+#                 */
/*   Updated: 2021/04/02 16:05:01 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPE_TRAITS_HPP
#define TYPE_TRAITS_HPP

#include <cstddef>

// See README.md for description of various traits

namespace meta {
template <typename T, T val>
    struct integral_constant {
        static const T value = val;
        typedef T value_type;
        typedef integral_constant type;
        operator value_type() const { return this->value; }
    };

template <typename T> struct alignment_of;

template <typename T>
    struct alignment_of_hack {
        char c;
        T t;
        alignment_of_hack();
    };

template <unsigned A, unsigned S>
    struct alignment_logic {
        static const size_t value = A < S ? A : S;
    };

template <typename T>
struct alignment_of_impl {
    static const size_t value =
        (alignment_logic<sizeof(alignment_of_hack<T>) - sizeof(T), sizeof(T)>::value);
};

template <typename T> struct alignment_of : integral_constant<size_t, alignment_of_impl<T>::value> {};

template <bool c, typename T1, typename T2>
struct if_c {
    typedef T1 type;
};

template <typename T1, typename T2>
struct if_c<false, T1, T2> {
    typedef T2 type;
};

template <bool found, size_t target, class TestType>
struct lower_alignment_helper {
    typedef char type;
    enum { value = true };
};

template <size_t target, class TestType>
struct lower_alignment_helper<false, target, TestType> {
    enum { value = (alignment_of<TestType>::value == target) };
    typedef typename if_c<value, TestType, char>::type type;
};

template <typename T>
struct has_one_T {
    T data;
};

class alignment_dummy;
typedef void (*function_ptr)();
typedef int (alignment_dummy::*member_ptr);
typedef int (alignment_dummy::*member_function_ptr)();

// Lifted in large part from Boost but w/o macros
template <size_t target>
union lower_alignment {
    enum { found0 = false };

    typename lower_alignment_helper< found0,target,char >::type t0;
 enum { found1 = lower_alignment_helper<found0,target,char >::value };
 typename lower_alignment_helper< found1,target,short >::type t1;
 enum { found2 = lower_alignment_helper<found1,target,short >::value };
 typename lower_alignment_helper< found2,target,int >::type t2;
 enum { found3 = lower_alignment_helper<found2,target,int >::value };
 typename lower_alignment_helper< found3,target,long >::type t3;
 enum { found4 = lower_alignment_helper<found3,target,long >::value };
 typename lower_alignment_helper< found4,target,long long >::type t4;
 enum { found5 = lower_alignment_helper<found4,target,long long >::value };
 typename lower_alignment_helper< found5,target,float >::type t5;
 enum { found6 = lower_alignment_helper<found5,target,float >::value };
 typename lower_alignment_helper< found6,target,double >::type t6;
 enum { found7 = lower_alignment_helper<found6,target,double >::value };
 typename lower_alignment_helper< found7,target,long double >::type t7;
 enum { found8 = lower_alignment_helper<found7,target,long double >::value };
 typename lower_alignment_helper< found8,target,void* >::type t8;
 enum { found9 = lower_alignment_helper<found8,target,void* >::value };
 typename lower_alignment_helper< found9,target,function_ptr >::type t9;
 enum { found10 = lower_alignment_helper<found9,target,function_ptr >::value };
 typename lower_alignment_helper< found10,target,member_ptr >::type t10;
 enum { found11 = lower_alignment_helper<found10,target,member_ptr >::value };
 typename lower_alignment_helper< found11,target,member_function_ptr >::type t11;
 enum { found12 = lower_alignment_helper<found11,target,member_function_ptr >::value };
 typename lower_alignment_helper< found12,target,has_one_T< char > >::type t12;
 enum { found13 = lower_alignment_helper<found12,target,has_one_T< char > >::value };
 typename lower_alignment_helper< found13,target,has_one_T< short > >::type t13;
 enum { found14 = lower_alignment_helper<found13,target,has_one_T< short > >::value };
 typename lower_alignment_helper< found14,target,has_one_T< int > >::type t14;
 enum { found15 = lower_alignment_helper<found14,target,has_one_T< int > >::value };
 typename lower_alignment_helper< found15,target,has_one_T< long > >::type t15;
 enum { found16 = lower_alignment_helper<found15,target,has_one_T< long > >::value };
 typename lower_alignment_helper< found16,target,has_one_T< long long > >::type t16;
 enum { found17 = lower_alignment_helper<found16,target,has_one_T< long long > >::value };
 typename lower_alignment_helper< found17,target,has_one_T< float > >::type t17;
 enum { found18 = lower_alignment_helper<found17,target,has_one_T< float > >::value };
 typename lower_alignment_helper< found18,target,has_one_T< double > >::type t18;
 enum { found19 = lower_alignment_helper<found18,target,has_one_T< double > >::value };
 typename lower_alignment_helper< found19,target,has_one_T< long double > >::type t19;
 enum { found20 = lower_alignment_helper<found19,target,has_one_T< long double > >::value };
 typename lower_alignment_helper< found20,target,has_one_T< void* > >::type t20;
 enum { found21 = lower_alignment_helper<found20,target,has_one_T< void* > >::value };
 typename lower_alignment_helper< found21,target,has_one_T< function_ptr > >::type t21;
 enum { found22 = lower_alignment_helper<found21,target,has_one_T< function_ptr > >::value };
 typename lower_alignment_helper< found22,target,has_one_T< member_ptr > >::type t22;
 enum { found23 = lower_alignment_helper<found22,target,has_one_T< member_ptr > >::value };
 typename lower_alignment_helper< found23,target,has_one_T< member_function_ptr > >::type t23;
 enum { found24 = lower_alignment_helper<found23,target,has_one_T< member_function_ptr > >::value };
};

union max_align {
    char t0;
 short t1;
 int t2;
 long t3;
 long long t4;
 float t5;
 double t6;
 long double t7;
 void* t8;
 function_ptr t9;
 member_ptr t10;
 member_function_ptr t11;
 has_one_T< char > t12;
 has_one_T< short > t13;
 has_one_T< int > t14;
 has_one_T< long > t15;
 has_one_T< long long > t16;
 has_one_T< float > t17;
 has_one_T< double > t18;
 has_one_T< long double > t19;
 has_one_T< void* > t20;
 has_one_T< function_ptr > t21;
 has_one_T< member_ptr > t22;
 has_one_T< member_function_ptr > t23;
};

template<size_t TAlign, size_t Align>
struct is_aligned {
    static const bool value = ((TAlign >= Align) & (TAlign % Align)) == 0;
};

template <size_t Align>
class type_with_alignment
{
    typedef lower_alignment<Align> t1;
    typedef typename if_c<
          is_aligned< alignment_of<t1>::value,Align >::value
        , t1
        , max_align
        >::type align_t;

    static const size_t found = alignment_of<align_t>::value;

 public:
    typedef align_t type;
};

} // namespace meta

#endif
