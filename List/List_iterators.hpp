/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   List_iterators.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/15 14:57:07 by alienard          #+#    #+#             */
/*   Updated: 2021/05/20 20:25:49 by alienard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <string>
#include <limits>
#include "iterator_traits.hpp"
#include "iterator.hpp"
#include "List_node.hpp"

namespace ft {

	template<typename NODE_TPE, typename VAL_TPE, class iterator_tag = ft::bidirectional_iterator_tag>
	class listIterator{
		public :
			typedef NODE_TPE						node_type;
			typedef node_type						&node_reference;
			typedef node_type						*node_pointer;
			typedef std::ptrdiff_t					difference_type;
			typedef iterator_tag					iterator_category;
			typedef VAL_TPE							value_type;
			typedef value_type						&reference;
			typedef value_type						*pointer;
		private :
			node_pointer							node;
		public :
			listIterator(){
				this->node = NULL;
			}
			~listIterator(){}
			listIterator(const listIterator &lst_it){
				*this = lst_it;
			}
			listIterator(node_pointer node){
				this->node = node;
			}
			listIterator(value_type node){
				this->node->content = node;
			}
			listIterator(const listIterator *listIterator){
				*this = listIterator;
			}

			operator listIterator<node_type, const value_type>(void) const {
				return listIterator<node_type, const value_type>(this->node);
			}
			
			node_pointer	get_node() const{
				return (this->node);
			}

			reference operator*() const{
				return (this->node->content);
			}
			pointer operator->() const{
				return &(this->operator*());
			}
			listIterator<node_type, value_type> &operator=(const listIterator<node_type, value_type> &rhs){
				this->node = rhs.get_node();
				return (*this);
			}

			listIterator<node_type, value_type>  &operator++(){
				node = node->next;
				return *this;
			}
			listIterator<node_type, value_type>  operator++(int){
				listIterator<node_type, value_type> __tmp(*this);
				node = node->next;
				return __tmp;
			}
			listIterator<node_type, value_type>  &operator--(){
				node = node->prev;
				return *this;
			}
			listIterator<node_type, value_type>  operator--(int){
				listIterator<node_type, value_type> __tmp(*this);
				node = node->prev;
				return __tmp;
			}
			node_pointer base() const{
				return node;
			}

			template <class , class , class > friend class listIterator;
			template <class , class > friend class list;

			template <class T>
			bool operator!=(const listIterator<node_type, T> &rhs) const {
				return (rhs.get_node()->content != this->get_node()->content
					|| rhs.get_node()->prev != this->get_node()->prev
					|| rhs.get_node()->next != this->get_node()->next);
			}
			template <class T>
			bool operator==(const listIterator<node_type, T> &rhs) const
			{
				return (rhs.get_node()->content == this->get_node()->content
						&& rhs.get_node()->next == this->get_node()->next
						&& rhs.get_node()->prev == this->get_node()->prev);
			}

	};

	template <class _Iter>
	class listreverseIterator
	{
		private:
			/*mutable*/ _Iter __t;  // no longer used as of LWG #2360, not removed due to ABI break
		protected:
			_Iter current;
		public:
			typedef _Iter                                            iterator_type;
			typedef typename iterator_traits<_Iter>::difference_type difference_type;
			typedef typename iterator_traits<_Iter>::reference       reference;
			typedef typename iterator_traits<_Iter>::pointer         pointer;
		listreverseIterator() : __t(), current() {}
		explicit listreverseIterator(_Iter __x) : __t(__x), current(__x) {}
		template <class _Up>

			listreverseIterator(const listreverseIterator<_Up>& __u) : __t(__u.base()), current(__u.base()) {}
		template <class _Up>

			listreverseIterator& operator=(const listreverseIterator<_Up>& __u)
				{ __t = current = __u.base(); return *this; }
		_Iter base() const {return current;}
		reference operator*() const {_Iter __tmp = current; return *--__tmp;}
		pointer  operator->() const {return &(operator*());}
		listreverseIterator& operator++() {--current; return *this;}
		listreverseIterator  operator++(int) {listreverseIterator __tmp(*this); --current; return __tmp;}
		listreverseIterator& operator--() {++current; return *this;}
		listreverseIterator  operator--(int) {listreverseIterator __tmp(*this); ++current; return __tmp;}
	
	};
	template <class _Iter1, class _Iter2>
	bool
	operator==(const listreverseIterator<_Iter1>& __x, const listreverseIterator<_Iter2>& __y)
	{
		return __x.base() == __y.base();
	}

	template <class _Iter1, class _Iter2>
	bool
	operator!=(const listreverseIterator<_Iter1>& __x, const listreverseIterator<_Iter2>& __y)
	{
		return __x.base() != __y.base();
	}

	template< class InputIt, class Distance >
	void advance( InputIt& it, Distance n ){
		for (Distance i = 0 ; i < n ; i++){
			it++;
		}
	}
}
