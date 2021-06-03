/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utility.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/23 09:53:57 by alienard          #+#    #+#             */
/*   Updated: 2021/05/20 20:44:01 by alienard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <string>
#include <limits>

namespace ft {

	template <class _T1, class _T2>
	struct pair
	{
		typedef _T1 first_type;
		typedef _T2 second_type;

		_T1 first;
		_T2 second;

		pair(void) : first(), second() { };
		pair(const first_type &a, const second_type &b) : first(a), second(b) { };
		template<class U, class V>
		pair(const pair<U, V> &src) : first(src.first), second(src.second) { };

		pair& operator=(pair & __p){
			first = __p.first;
			second = __p.second;
			return *this;
		}

		pair& operator=(pair const& __p) {
			first = __p.first;
			second = __p.second;
			return *this;
		}
	};

	template <class T1,class T2>
	pair<T1,T2> make_pair (T1 x, T2 y)
	{
		return ( pair<T1,T2>(x,y) );
	}

	template <class _T1, class _T2>
	bool operator==(const pair<_T1,_T2>& __x, const pair<_T1,_T2>& __y) {
		return __x.first == __y.first && __x.second == __y.second;
	}

	template <class _T1, class _T2>
	bool operator!=(const pair<_T1,_T2>& __x, const pair<_T1,_T2>& __y) {
		return !(__x == __y);
	}

	template <class _T1, class _T2>
	bool operator< (const pair<_T1,_T2>& __x, const pair<_T1,_T2>& __y){
		return __x.first < __y.first || (!(__y.first < __x.first) && __x.second < __y.second);
	}

	template <class _T1, class _T2>
	bool operator> (const pair<_T1,_T2>& __x, const pair<_T1,_T2>& __y) {
		return __y < __x;
	}

	template <class _T1, class _T2>
	bool operator>=(const pair<_T1,_T2>& __x, const pair<_T1,_T2>& __y){
		return !(__x < __y);
	}

	template <class _T1, class _T2>
	bool operator<=(const pair<_T1,_T2>& __x, const pair<_T1,_T2>& __y){
		return !(__y < __x);
	}

	template<bool B, class T = void>
	struct enable_if {};
	
	template<class T>
	struct enable_if<true, T> { typedef T type; };

}
