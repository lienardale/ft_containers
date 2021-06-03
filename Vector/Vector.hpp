/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Vector.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/29 19:36:39 by alienard          #+#    #+#             */
/*   Updated: 2021/05/20 20:40:24 by alienard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "algorithm.hpp"
#include "iterator_traits.hpp"
#include "iterator.hpp"

namespace ft
{
	template< class T, class Allocator = std::allocator<T> >
	class vector
	{
		public:
			typedef T										value_type;
			typedef Allocator								allocator_type;
			typedef value_type&								reference;
			typedef const value_type&						const_reference;
			typedef value_type*								pointer;
			typedef const value_type*						const_pointer;
			typedef std::ptrdiff_t							difference_type;
			typedef std::size_t								size_type;

			typedef ft::__wrap_iter<pointer>				iterator;
			typedef ft::__wrap_iter<const_pointer>			const_iterator;
			typedef ft::reverse_iterator<iterator>			reverse_iterator;
			typedef ft::reverse_iterator<const_iterator>	const_reverse_iterator;


		private:
			pointer					_container;
			size_type				_size;
			size_type				_capacity;
			allocator_type			_alloc;

		size_type new_capacity (size_type new_size)
		{
			size_type ret = _size;
			if (ret == 0 && _size == 0)
				ret = new_size;
			while (ret < new_size){
				ret = ret * 2;
			}
			return (ret);
		}

		void realloc (size_type n)
		{
			pointer tmp = this->_alloc.allocate(this->_capacity);
			
			for (size_type i = 0; i < this->size(); i++) {
				this->_alloc.construct(&tmp[i], this->_container[i]);
				this->_alloc.destroy(&this->_container[i]);
			}
			this->_alloc.deallocate(this->_container, n);
			this->_container = tmp;
		}

		public:
			// default (1)	
			explicit vector (const allocator_type& alloc = allocator_type()){
				_container = NULL;
				_size = 0;
				_capacity = 0;
				_alloc = alloc;
			}
			// fill (2)	
			explicit vector (size_type n, const value_type& val = value_type(), const allocator_type& alloc = allocator_type()){
				_alloc = alloc;
				_capacity = 0;
				_container = NULL;
				_container = _alloc.allocate(_capacity);
				_size = 0;
				this->assign(n, val);
			}
			// range (3)	
			template <class InputIterator>
			vector (InputIterator first, InputIterator last,
					const allocator_type& alloc = allocator_type(),
					typename ft::__void_t<typename ft::iterator_traits<InputIterator>::iterator_category>::type* = 0){
				_alloc = alloc;
				_capacity = 0;
				_container = NULL;
				_container = _alloc.allocate(_capacity);
				_size = 0;
				this->assign(first, last);
			}
			// copy (4)	
			vector (const vector& x){
				_alloc = x.get_allocator();
				_capacity = 0;
				_container = NULL;
				_container = _alloc.allocate(0);
				_size = 0;
				this->assign(x.begin(), x.end());
			}
			~vector(){
				this->clear();
				this->_alloc.deallocate(this->_container, this->capacity());
			}
			// assign op
 			vector& operator= (const vector& x){
				this->clear();
				if (this->_container)
					this->_alloc.deallocate(this->_container, this->capacity());
				_alloc = x.get_allocator();
				_capacity = (x.capacity() < this->capacity() ) ? this->capacity() : 0;
				_container = _alloc.allocate(_capacity);
				_size = 0;
				this->assign(x.begin(), x.end());
				return *this;
			}

			allocator_type get_allocator() const{
				return this->_alloc; 
			}
			
			iterator begin() {
				return (iterator(&this->_container[0])); 
			}
			const_iterator begin() const{
				return (const_iterator(&this->_container[0])); 
			}

			iterator end(){
				return (iterator(&this->_container[this->_size])); 
			}
			const_iterator end() const{
				return (const_iterator(&this->_container[this->_size])); 
			}

			reverse_iterator rbegin(){
				return (reverse_iterator(end())); 
			}
			const_reverse_iterator rbegin() const{
				return (const_reverse_iterator(end())); 
			}

			reverse_iterator rend(){
				return (reverse_iterator(begin())); 
			}
			const_reverse_iterator rend() const{
				return (const_reverse_iterator(begin())); 
			}

			size_type size() const{
				return this->_size;
			}

			size_type max_size() const{
				return this->_alloc.max_size();
			}

			void resize (size_type n, value_type val = value_type()){
				if (n > this->_size){
					if (n < _size * 2) {
						if (n > this->max_size())
							throw (std::length_error("new requested size is upper than the max size"));
						size_type old_capacity = this->_capacity;
						_capacity = new_capacity(n);
						this->realloc(old_capacity);
					}
					else
						this->reserve(n);
					for (size_type i = this->size(); i < n; i++)
						this->_alloc.construct(_alloc.address(this->_container[i]), val);
					this->_size = n;
				}
				else
					erase(this->begin() + n, this->end());
			}

			size_type capacity() const{
				return this->_capacity;
			}

			bool empty() const{
				return ((this->_size == 0) ? true : false);
			}

			void reserve (size_type n){
				if (n > _capacity) {
					if (n > this->max_size())
						throw (std::length_error("new requested size is upper than the max size"));
					size_type old_capacity = this->_capacity;
					_capacity = n;
					this->realloc(old_capacity);
				}
			}

			reference operator[] (size_type n){
				return (this->_container[n]);
			}
			const_reference operator[] (size_type n) const{
				return (this->_container[n]);
			}

			reference at (size_type n){
				if (n >= this->_size){
					throw std::out_of_range("vector");
				}
				return (this->_container[n]);
			}
			const_reference at (size_type n) const{
				if (n >= this->_size)
					throw std::out_of_range("vector");
				return (*(this->_container + n));
			}

			reference front(){
				return (this->at(0));
			}
			const_reference front() const{
				return (this->at(0));
			}

			reference back(){
				if (this->empty() == true)
					return (this->front());
				return (this->at(this->size() - 1));
			}
			const_reference back() const{
				if (this->empty() == true)
					return (this->front());
				return (this->at(this->size() - 1));
			}

			// range (1)	
			template <class InputIterator>
			void assign (InputIterator first, InputIterator last,
						typename ft::__void_t<typename ft::iterator_traits<InputIterator>::iterator_category>::type* = 0){
				this->clear();
				size_type len = ft::iterator_range_size(first, last);
				if ( this->_size + len > this->_capacity ){
					if ( len < _capacity * 2)
						this->reserve(len);
					else
						this->reserve(new_capacity(len));
				}
				for (; first != last ; first++){
					this->_alloc.construct(_alloc.address(this->_container[this->_size]), *first);
					this->_size++;
				}
			}
			// fill (2)	
			void assign (size_type n, const value_type& val = value_type()){
				this->clear();
				if ( this->_size + n > this->_capacity ){
					if ( n < _capacity * 2)
						this->reserve(n);
					else
						this->reserve(new_capacity(n));
				}
				for (size_type i = 0 ; i < n ; i++){
					this->_alloc.construct(_alloc.address(this->_container[i]), val);
					this->_size++;
				}
			}

			void push_back (const value_type& val){
				if (this->_size + 1 > this->capacity()){
					reserve(new_capacity(this->_size + 1));
				}
				this->_alloc.construct(_alloc.address(this->_container[this->_size]), val);
				this->_size++;
			}

			void pop_back(){
				if (!empty()){
					this->_alloc.destroy(&this->_container[size() - 1]);
					this->_size--;
				}
			}

			// single element (1)	
			iterator insert (iterator position, const value_type& val){
				difference_type	ret = position - this->begin();
				this->insert(position, 1, val);
				return this->begin() + ret;
			}
			// fill (2)	
			void insert (iterator position, size_type n, const value_type& val){
				vector tmp(position, end());
				size_type old_size = _size;
				_size -= end() - position;

				if (old_size + n > _size * 2){
					size_type new_cap = old_size;
					if (_size == 0)
						new_cap = old_size + n;
					while (new_cap < old_size + n){
						new_cap = new_cap * 2;
					}
					reserve(new_cap);
				}
				else
					reserve(old_size + n);
				for (size_type i = 0; i < n; i++) {
					this->_container[this->_size] = val;
					this->_size++;
				}
				for (iterator it = tmp.begin(); it != tmp.end(); it++) {
					this->_container[this->_size] = *it;
					this->_size++;
				}
			}
			// range (3)	
			template <class InputIterator>
			void insert (iterator position, InputIterator first, InputIterator last,
							typename ft::__void_t<typename ft::iterator_traits<InputIterator>::iterator_category>::type* = 0){
				vector tmp(position, end());
				size_type old_size = _size;
				erase(position, end());
				size_type n = ft::iterator_range_size(first, last);
				if (old_size + n > _capacity * 2){
					reserve(old_size + n);
				}
				else{
					size_type new_cap = old_size;
					if (new_cap == 0 && _size == 0)
						new_cap = old_size + n;
					while (new_cap < old_size + n){
						new_cap = new_cap * 2;
					}
					reserve(new_cap);

				}
				for (; first != last ; first++) {
					this->_container[this->_size] = *first;
					this->_size++;
				}
				for (iterator it = tmp.begin(); it != tmp.end(); it++) {
					this->_container[this->_size] = *it;
					this->_size++;
				}
			}

			iterator erase (iterator position){
				iterator	ret = position;

				for (iterator	it = position + 1; it != this->end() ; it++){
					*position = *it;
					position++;
				}
				this->_size--;
				return ret;
			}
			iterator erase (iterator first, iterator last){
				for (; first != last ; last--){
					erase(first);
				}
				return first;
			}

			void swap (vector& x){
				ft::swap(this->_alloc, x._alloc);
				ft::swap(this->_container, x._container);
				ft::swap(this->_capacity, x._capacity);
				ft::swap(this->_size, x._size);
			}

			void clear(){
				erase(this->begin(), this->end());
				this->_size = 0;
			}
	};

	template <class T, class Alloc>
	void swap (vector<T,Alloc>& x, vector<T,Alloc>& y){
		x.swap(y);
	}

/*
Performs the appropriate comparison operation between the vector containers lhs and rhs.

The equality comparison (operator==) is performed by first comparing sizes, and if they match, the elements are compared sequentially using operator==, stopping at the first mismatch (as if using algorithm equal).

The less-than comparison (operator<) behaves as if using algorithm lexicographical_compare, which compares the elements sequentially using operator< in a reciprocal manner (i.e., checking both a<b and b<a) and stopping at the first occurrence.

The other operations also use the operators == and < internally to compare the elements, behaving as if the following equivalent operations were performed:
operation	equivalent operation
a!=b	!(a==b)
a>b		b<a
a<=b	!(b<a)
a>=b	!(a<b)
*/

	template <class T, class Alloc>
	bool operator== (const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs){
		if (lhs.size() != rhs.size())
			return false ;
		ft::__wrap_iter<const T*> itl = lhs.begin();
		ft::__wrap_iter<const T*> itr = rhs.begin();
		while (itl != lhs.end()){
			if (*itl != *itr)
				return false;
			itl++;
			itr++;
		}
		return true;
	}
	template <class T, class Alloc>
	bool operator!= (const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs){
		return !(lhs==rhs);
	}
	template <class T, class Alloc>
	bool operator< (const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs){
		return ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}
	template <class T, class Alloc>
	bool operator<= (const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs){
		return !(rhs<lhs);
	}
	template <class T, class Alloc>
	bool operator> (const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs){
		return rhs<lhs;
	}
	template <class T, class Alloc>
	bool operator>= (const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs){
		return !(lhs<rhs);
	}
}
