/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   algorithm.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/13 12:10:58 by alienard          #+#    #+#             */
/*   Updated: 2021/05/20 20:40:44 by alienard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <string>
#include <limits>
// https://en.cppreference.com/w/cpp/algorithm

namespace ft {

	template< class InputIt1, class InputIt2 >
	bool lexicographical_compare( InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2 ){
		for ( ; (first1 != last1) && (first2 != last2); ++first1, (void) ++first2 ) {
			if (*first1 < *first2) return true;
			if (*first2 < *first1) return false;
		}
		return (first1 == last1) && (first2 != last2);
	}

	template< class T >
	struct less {
		bool operator()(const T& lhs, const T& rhs ) const {
			return lhs < rhs;
		}
	};

	template< class T >
	void swap( T& a, T& b ){
		T tmp = a;
		a = b;
		b = tmp;
	}

	template <class Iterator>
	size_t	iterator_range_size(Iterator first, Iterator last) {
		size_t	size = 0 ;
		for (; first != last ; first++){
			size++;
		}
		return (size);
	}
}
