/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   iterator.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/11 16:08:01 by alienard          #+#    #+#             */
/*   Updated: 2021/05/20 20:43:12 by alienard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <string>
#include <limits>
#include "iterator_traits.hpp"
#include "utility.hpp"

namespace ft {

	template<class _Category, class _Tp, class _Distance = ptrdiff_t, class _Pointer = _Tp*, class _Reference = _Tp&>
	struct iterator
	{
		typedef _Tp			value_type;
		typedef _Distance	difference_type;
		typedef _Pointer	pointer;
		typedef _Reference	reference;
		typedef _Category	iterator_category;
	};

	template <class _Iter>
	class __wrap_iter
	{
		public:
			typedef _Iter														iterator_type;
			typedef typename iterator_traits<iterator_type>::iterator_category	iterator_category;
			typedef typename iterator_traits<iterator_type>::value_type			value_type;
			typedef typename iterator_traits<iterator_type>::difference_type	difference_type;
			typedef typename iterator_traits<iterator_type>::pointer			pointer;
			typedef typename iterator_traits<iterator_type>::reference			reference;
		private:
			iterator_type __i;
		public:
			__wrap_iter(): __i(){
				
			}
			template <class _Up>
			__wrap_iter(const __wrap_iter<_Up>& __u) : __i(__u.base()){}
			
			__wrap_iter(iterator_type __x) : __i(__x) {}

			reference operator*(){return *__i;}
			pointer operator->(){return &(*__i);}
			__wrap_iter& operator++(){
				++__i;
				return *this;
			}
			__wrap_iter operator++(int){
				__wrap_iter __tmp(*this);
				++(*this);
				return __tmp;
			}
			__wrap_iter& operator--(){
				--__i;
				return *this;
			}
			__wrap_iter operator--(int){
				__wrap_iter __tmp(*this);
				--(*this);
				return __tmp;
			}
			__wrap_iter operator+ (difference_type __n) const{
				__wrap_iter __w(*this);
				__w += __n;
				return __w;
			}
			__wrap_iter& operator+=(difference_type __n){
				__i += __n;
				return *this;
			}
			__wrap_iter operator- (difference_type __n) const{
				return *this + (-__n);
			}
			__wrap_iter& operator-=(difference_type __n){
				*this += -__n;
				return *this;
			}
			reference operator[](difference_type __n) const{
				return __i[__n];
			}
			iterator_type base() const{
				return __i;
			}

			template <class _Up> friend class __wrap_iter;
			template <class _Tp, class _Alloc> friend class vector;

			template <class _Iter1, class _Iter2> friend
			bool operator==(const __wrap_iter<_Iter1>&, const __wrap_iter<_Iter2>&);
			template <class _Iter1, class _Iter2> friend
			bool operator<(const __wrap_iter<_Iter1>&, const __wrap_iter<_Iter2>&);
			template <class _Iter1, class _Iter2> friend
			bool operator!=(const __wrap_iter<_Iter1>&, const __wrap_iter<_Iter2>&);
			template <class _Iter1, class _Iter2> friend
			bool operator>(const __wrap_iter<_Iter1>&, const __wrap_iter<_Iter2>&);
			template <class _Iter1, class _Iter2> friend
			bool operator>=(const __wrap_iter<_Iter1>&, const __wrap_iter<_Iter2>&);
			template <class _Iter1, class _Iter2> friend
			bool operator<=(const __wrap_iter<_Iter1>&, const __wrap_iter<_Iter2>&);
			template <class _Iter1, class _Iter2> friend
			bool operator+(const __wrap_iter<_Iter1>&, const __wrap_iter<_Iter2>&);
	};
	template <class _Iter1, class _Iter2>
	bool operator==(const __wrap_iter<_Iter1>& __x, const __wrap_iter<_Iter2>& __y)
	{
		return __x.base() == __y.base();
	}
	template <class _Iter1, class _Iter2>
	bool operator<(const __wrap_iter<_Iter1>& __x, const __wrap_iter<_Iter2>& __y)
	{
		return __x.base() < __y.base();
	}
	template <class _Iter1, class _Iter2>
	bool operator!=(const __wrap_iter<_Iter1>& __x, const __wrap_iter<_Iter2>& __y) 
	{
		return !(__x == __y);
	}
	template <class _Iter1, class _Iter2>
	bool operator>(const __wrap_iter<_Iter1>& __x, const __wrap_iter<_Iter2>& __y)
	{
		return __y < __x;
	}

	template <class _Iter1, class _Iter2>
	bool operator>=(const __wrap_iter<_Iter1>& __x, const __wrap_iter<_Iter2>& __y)
	{
		return !(__x < __y);
	}

	template <class _Iter1, class _Iter2>
	bool operator<=(const __wrap_iter<_Iter1>& __x, const __wrap_iter<_Iter2>& __y)
	{
		return !(__y < __x);
	}

	template <class _Iter1>
	bool operator!=(const __wrap_iter<_Iter1>& __x, const __wrap_iter<_Iter1>& __y)
	{
		return !(__x == __y);
	}

	template <class _Iter1>
	bool operator>(const __wrap_iter<_Iter1>& __x, const __wrap_iter<_Iter1>& __y)
	{
		return __y < __x;
	}

	template <class _Iter1>
	bool operator>=(const __wrap_iter<_Iter1>& __x, const __wrap_iter<_Iter1>& __y)
	{
		return !(__x < __y);
	}

	template <class _Iter1>
	bool operator<=(const __wrap_iter<_Iter1>& __x, const __wrap_iter<_Iter1>& __y)
	{
		return !(__y < __x);
	}

	template <class _Iter1, class _Iter2>
	typename __wrap_iter<_Iter1>::difference_type operator-(const __wrap_iter<_Iter1>& __x, const __wrap_iter<_Iter2>& __y)
	{
		return __x.base() - __y.base();
	}

	template <class _Iter>
	__wrap_iter<_Iter> operator+(typename __wrap_iter<_Iter>::difference_type __n, __wrap_iter<_Iter> __x)
	{
		__x += __n;
		return __x;
	}

	template <class _Iter>
	class reverse_iterator
		: public ft::iterator<typename iterator_traits<_Iter>::iterator_category,
						typename iterator_traits<_Iter>::value_type,
						typename iterator_traits<_Iter>::difference_type,
						typename iterator_traits<_Iter>::pointer,
						typename iterator_traits<_Iter>::reference>
	{
		private:
			/*mutable*/ _Iter __t;  // no longer used as of LWG #2360, not removed due to ABI break

		protected:
			_Iter current;
		public:
			typedef _Iter												iterator_type;
			typedef typename iterator_traits<_Iter>::difference_type	difference_type;
			typedef typename iterator_traits<_Iter>::reference			reference;
			typedef typename iterator_traits<_Iter>::pointer			pointer;

			reverse_iterator() : __t(), current() {}
			explicit reverse_iterator(_Iter __x) : __t(__x), current(__x) {}
			template <class _Up>

				reverse_iterator(const reverse_iterator<_Up>& __u) : __t(__u.base()), current(__u.base()) {}
			template <class _Up>

				reverse_iterator& operator=(const reverse_iterator<_Up>& __u)
					{ __t = current = __u.base(); return *this; }
			_Iter base() const {return current;}
			reference operator*() const {_Iter __tmp = current; return *--__tmp;}
			pointer operator->() const {return &(operator*());}
			reverse_iterator& operator++() {--current; return *this;}
			reverse_iterator operator++(int) {reverse_iterator __tmp(*this); --current; return __tmp;}
			reverse_iterator& operator--() {++current; return *this;}
			reverse_iterator operator--(int) {reverse_iterator __tmp(*this); ++current; return __tmp;}
			reverse_iterator operator+ (difference_type __n) const {return reverse_iterator(current - __n);}
			reverse_iterator& operator+=(difference_type __n) {current -= __n; return *this;}
			reverse_iterator operator- (difference_type __n) const {return reverse_iterator(current + __n);}
			reverse_iterator& operator-=(difference_type __n) {current += __n; return *this;}
			reference operator[](difference_type __n) const {return *(*this + __n);}
	};

	template <class _Iter1, class _Iter2>
	bool
	operator==(const reverse_iterator<_Iter1>& __x, const reverse_iterator<_Iter2>& __y)
	{
		return __x.base() == __y.base();
	}

	template <class _Iter1, class _Iter2>
	bool
	operator<(const reverse_iterator<_Iter1>& __x, const reverse_iterator<_Iter2>& __y)
	{
		return __x.base() > __y.base();
	}

	template <class _Iter1, class _Iter2>
	bool
	operator!=(const reverse_iterator<_Iter1>& __x, const reverse_iterator<_Iter2>& __y)
	{
		return __x.base() != __y.base();
	}

	template <class _Iter1, class _Iter2>
	bool
	operator>(const reverse_iterator<_Iter1>& __x, const reverse_iterator<_Iter2>& __y)
	{
		return __x.base() < __y.base();
	}

	template <class _Iter1, class _Iter2>
	bool
	operator>=(const reverse_iterator<_Iter1>& __x, const reverse_iterator<_Iter2>& __y)
	{
		return __x.base() <= __y.base();
	}

	template <class _Iter1, class _Iter2>
	bool
	operator<=(const reverse_iterator<_Iter1>& __x, const reverse_iterator<_Iter2>& __y)
	{
		return __x.base() >= __y.base();
	}

	template <class _Iter1, class _Iter2>
	typename reverse_iterator<_Iter1>::difference_type
	operator-(const reverse_iterator<_Iter1>& __x, const reverse_iterator<_Iter2>& __y)
	{
		return __y.base() - __x.base();
	}

	template <class _Iter>
	reverse_iterator<_Iter>
	operator+(typename reverse_iterator<_Iter>::difference_type __n, const reverse_iterator<_Iter>& __x)
	{
		return reverse_iterator<_Iter>(__x.base() - __n);
	}

}
