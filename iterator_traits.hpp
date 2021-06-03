/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   iterator_traits.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/12 13:30:34 by alienard          #+#    #+#             */
/*   Updated: 2021/05/18 10:29:00 by alienard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <cstddef>	//for ptrdiff_t
// #include <type_traits> //for __void_t
/*

Container		it_type supported
Vector			random_access
List			bidirectional
Map				bidirectional
Stack			no iterator supported
Queue			no iterator supported

It				Properties
bidirectional	= *it ,  *it = , ++ , -- , == , !=
random_access	-> , [] , = *it ,  *it = , ++ , -- , += , -= , + , - , == , != , < , > , <= , >=

*/

namespace ft {

	template <class>
	struct __void_t {typedef void type;};

// As implemented in gcc/libstdc++-v3/include/bits/stl_iterator_base_types.h

	// https://www.cplusplus.com/reference/iterator/InputIterator/
	struct input_iterator_tag {};
	// https://www.cplusplus.com/reference/iterator/OutputIterator/
	struct output_iterator_tag {};
	// https://www.cplusplus.com/reference/iterator/ForwardIterator/
	 struct forward_iterator_tag : public input_iterator_tag { };
	// https://www.cplusplus.com/reference/iterator/BidirectionalIterator/
	struct bidirectional_iterator_tag : public forward_iterator_tag { };
	// http://cplusplus.com/reference/iterator/RandomAccessIterator/
	struct random_access_iterator_tag : public bidirectional_iterator_tag { };
	
// // SFINAE test -> from https://gist.github.com/fenbf/d2cd670704b82e2ce7fd
// template <typename T>
// class HasToString
// {
// private:
//     typedef char YesType[1];
//     typedef char NoType[2];
//     template <typename C> static YesType& test( decltype(&C::ToString) ) ;
//     template <typename C> static NoType& test(...);
// public:
//     enum { value = sizeof(test<T>(0)) == sizeof(YesType) };
// };

	template<class T>
	struct iterator_typedefs_test {
		private :
			// struct s_test{
				typedef char _yes[1];
				typedef char _no[2];
			// };
			template <class U>	static _yes& test(typename ft::__void_t<typename U::iterator_category>::type * = 0,
										typename ft::__void_t<typename U::difference_type>::type * = 0,
										typename ft::__void_t<typename U::value_type>::type * = 0,
										typename ft::__void_t<typename U::reference>::type * = 0,
										typename ft::__void_t<typename U::pointer>::type * = 0);
			template <class U>	static _no& test(...);
		public:
			static const bool value = sizeof(test<T>(0,0,0,0,0)) == 1;
			// enum { value = sizeof(test<T>(0,0,0,0,0)) == sizeof(_yes) }; -> enum is C++11 in this case
	};

	template <class Iter, bool>
	struct _iterator_traits {};

	template <class Iter>
	struct _iterator_traits<Iter, true>
	{
		typedef	typename Iter::value_type			value_type;
		typedef	typename Iter::difference_type		difference_type;
		typedef	typename Iter::pointer				pointer;
		typedef	typename Iter::reference			reference;
		typedef	typename Iter::iterator_category	iterator_category;
	};

	template <class Iter>
	struct iterator_traits : public _iterator_traits<Iter, iterator_typedefs_test<Iter>::value> {};

// http://cplusplus.com/reference/iterator/iterator_traits/

	// template <class Iterator> class iterator_traits;

	template <class T>
	class iterator_traits<T*>
	{
		public :
			typedef T								value_type;
			typedef std::ptrdiff_t					difference_type;
			typedef T*								pointer;
			typedef T&								reference;
			typedef ft::random_access_iterator_tag	iterator_category;
	};

	template <class T>
	class iterator_traits<const T*>
	{
		public :
			typedef T								value_type;
			typedef std::ptrdiff_t					difference_type;
			typedef const T*						pointer;
			typedef const T&						reference;
			typedef ft::random_access_iterator_tag	iterator_category;
	};
}
