/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Map_iterators.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/21 17:45:53 by alienard          #+#    #+#             */
/*   Updated: 2021/05/20 20:28:30 by alienard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <string>
#include <limits>
#include "iterator_traits.hpp"
#include "iterator.hpp"
#include "Map_node.hpp"

namespace ft {

	template<typename NODE_TPE, typename KEY_TPE, class iterator_tag = ft::bidirectional_iterator_tag>
	class mapIterator{
		public :
			typedef KEY_TPE							value_type;
			typedef value_type						&reference;
			typedef value_type						*pointer;
			typedef NODE_TPE						node_type;
			typedef node_type						&node_reference;
			typedef node_type						*node_pointer;
			typedef std::ptrdiff_t					difference_type;
			typedef iterator_tag					iterator_category;
		private :
			node_pointer							node;
		public :
			mapIterator(){
				this->node = NULL;
			}
			~mapIterator(){}
			mapIterator(const mapIterator &lst_it){
				*this = lst_it;
			}
			mapIterator(node_pointer node){
				this->node = node;
			}
			mapIterator(value_type node){
				this->node->content = node;
			}

			mapIterator(const mapIterator *mapIterator){
				*this = mapIterator;
			}

			operator mapIterator<node_type, const value_type>(void) const {
				return mapIterator<node_type, const value_type>(this->node);
			}
			
			node_pointer	get_n() const{
				return (this->node);
			}

			reference operator*() const{
				return (this->node->data);
			}
			pointer operator->() const{
				return &(this->operator*());
			}
			mapIterator<node_type, value_type> &operator=(const mapIterator<node_type, value_type> &rhs){
				this->node = rhs.get_n();
				return (*this);
			}

			mapIterator<node_type, value_type>  &operator++(){
				node = node->getNext();
				return *this;
			}
			mapIterator<node_type, value_type>  operator++(int){
				mapIterator<node_type, value_type> __tmp(*this);
				node = node->getNext();
				return __tmp;
			}
			mapIterator<node_type, value_type>  &operator--(){
				node = node->getPrev();
				return *this;
			}
			mapIterator<node_type, value_type>  operator--(int){
				mapIterator<node_type, value_type> __tmp(*this);
				node = node->getPrev();
				return __tmp;
			}
			node_pointer base() const{
				return node;
			}

			template <class , class , class > friend class mapIterator;
			template <class , class , class , class > friend class map;

			template <class U>
			bool operator!=(const mapIterator<node_type, U> &rhs) const {
				return (rhs.node->data.first != this->node->data.first
					|| rhs.node->left != this->node->left
					|| rhs.node->right != this->node->right);
			}
			template <class U>
			bool operator==(const mapIterator<node_type, U> &rhs) const
			{
				return (rhs.node->data.first == this->node->data.first
						&& rhs.node->right == this->node->right
						&& rhs.node->left == this->node->left);
			}

	};

	template <class _Iter>
	class mapreverseIterator
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
		mapreverseIterator() : __t(), current() {}
		explicit mapreverseIterator(_Iter __x) : __t(__x), current(__x) {}
		template <class _Up>

			mapreverseIterator(const mapreverseIterator<_Up>& __u) : __t(__u.base()), current(__u.base()) {}
		template <class _Up>

			mapreverseIterator& operator=(const mapreverseIterator<_Up>& __u)
				{ __t = current = __u.base(); return *this; }
		_Iter base() const {return current;}
		reference operator*() const {_Iter __tmp = current; return *--__tmp;}
		pointer  operator->() const {return &(operator*());}
		mapreverseIterator& operator++() {--current; return *this;}
		mapreverseIterator  operator++(int) {mapreverseIterator __tmp(*this); --current; return __tmp;}
		mapreverseIterator& operator--() {++current; return *this;}
		mapreverseIterator  operator--(int) {mapreverseIterator __tmp(*this); ++current; return __tmp;}
	
	};
	template <class _Iter1, class _Iter2>
	bool
	operator==(const mapreverseIterator<_Iter1>& __x, const mapreverseIterator<_Iter2>& __y)
	{
		return __x.base() == __y.base();
	}

	template <class _Iter1, class _Iter2>
	bool
	operator!=(const mapreverseIterator<_Iter1>& __x, const mapreverseIterator<_Iter2>& __y)
	{
		return __x.base() != __y.base();
	}

}
