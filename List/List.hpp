/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   List.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/08 12:00:51 by alienard          #+#    #+#             */
/*   Updated: 2021/05/20 20:27:59 by alienard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "List_node.hpp"
#include "List_iterators.hpp"
#include "algorithm.hpp"
#include "utility.hpp"
#include "iterator_traits.hpp"

namespace ft
{
	template < class T, class Alloc = std::allocator<T> >
	class list
	{
		public:
			typedef T											value_type;
			typedef const value_type							const_value_type;
			typedef value_type									&reference;
			typedef const value_type							&const_reference;
			typedef value_type									*pointer;
			typedef const value_type							*const_pointer;
			typedef node<value_type>							node_type;
			typedef node<value_type>							*node_pointer;
			typedef Alloc										allocator_type;
			typedef listIterator<node_type, value_type>			iterator;
			typedef listIterator<node_type, const_value_type>	const_iterator;
			typedef listreverseIterator<iterator>				reverse_iterator;
			typedef listreverseIterator<const_iterator>			const_reverse_iterator;
			typedef std::size_t									size_type;
			typedef std::ptrdiff_t								difference_type;

		protected:
			node_pointer		l_begin;
			node_pointer		l_end;
			size_t				len;
			allocator_type		allocator;

		private:
			template<typename U>
			void swap(U &i, U &j){
				U tmp = j;
				j = i;
				i = tmp;
			}
			static bool less_than(value_type first, value_type second){
				return (first < second);
			}
			static bool are_equal(value_type first, value_type second){
				return (first == second);
			}
			node_pointer	get_node(size_type i) {
				size_type j = 0;
				for (iterator tmp = this->begin() ; tmp != this->end(); tmp++){
					if (i == j)
						return (tmp.get_node());
					j++;
				}
				return (NULL);
			}
		public:
			// defautlt constructor
			explicit list(const allocator_type &alloc = allocator_type()){
				this->l_end = new node<value_type> ();
				this->l_begin = this->l_end;
				this->l_end->prev = this->l_begin;
				this->l_begin->next = this->l_end;
				this->len = 0;
				this->allocator = alloc;
			}
			virtual ~list(){

				this->clear();
				delete this->l_end;
			}
			// fill constructor
			// Constructs a container with n elements. Each element is a copy of val.
			list(size_t n, const value_type& val = value_type(), const allocator_type& alloc = allocator_type()){
				this->l_end = new node<value_type> ();
				this->l_begin = this->l_end;
				this->l_end->prev = this->l_begin;
				this->l_begin->next = this->l_end;
				this->len = 0;
				this->allocator = alloc;
				for (size_t i = 0 ; i < n ; i++){
					this->push_back(val);
				}
			}
			// range constructor
			// Constructs a container with as many elements as the range [first,last),
			// with each element constructed from its corresponding element in that range, in the same order.
			template<class InputIterator>
			list(InputIterator first, InputIterator last, const allocator_type& alloc = allocator_type()){
				this->l_end = new node<value_type> ();
				this->l_begin = this->l_end;
				this->l_end->prev = this->l_begin;
				this->l_begin->next = this->l_end;
				this->len = 0;
				this->allocator = alloc;
				this->assign(first, last);
			}
			// copy constructor
			// Constructs a container with a copy of each of the elements in x, in the same order.
			list(const list &x){
				this->l_end = new node<value_type> ();
				this->l_begin = this->l_end;
				this->l_end->prev = this->l_begin;
				this->l_begin->next = this->l_end;
				this->len = 0;
				this->allocator = x.allocator;
				this->assign(x.begin(), x.end());
			}
			// assignation operator
			list &operator= (const list &x){
				this->clear();
				if (this->l_end)
					delete this->l_end;
				this->l_end = new node<value_type> ();
				this->l_begin = this->l_end;
				this->l_end->prev = this->l_begin;
				this->l_begin->next = this->l_end;
				this->len   = 0;
				this->allocator = x.allocator;
				this->assign(x.begin(), x.end());
				return (*this);
			}

			iterator	begin(){
				return (iterator(this->l_begin));
			}

			const_iterator begin() const {
				return (const_iterator(this->l_begin));
			}
			
			iterator	end(){
				return (iterator(this->l_end));
			}
			
			const_iterator	end() const {
				return (const_iterator(this->l_end));
			}
// reverse iterators
			reverse_iterator rbegin(){
				return (reverse_iterator(this->l_end));
			}

			const_reverse_iterator rbegin() const{
				return (const_reverse_iterator(this->l_end));
			}

			reverse_iterator rend(){
				return (reverse_iterator(this->l_begin));
			}

			const_reverse_iterator rend() const{
				return (const_reverse_iterator(this->l_begin));
			}
			
			bool	empty() const {
				return ((this->len == 0) ? true : false);
			}
			
			size_t	size() const {
				return (this->len);
			}


			size_t	max_size() const {
				// // 9223372036854775807 / 768614336404564650 = 12
				// // (void) lst;
				// if (std::is_integral<value_type>::value == true)
				// 	return std::numeric_limits<difference_type>::max() / 12;
				// else{
				// 	// 9223372036854775807 / 461168601842738790 = 20
				// 	return std::numeric_limits<difference_type>::max() / 20;
				// }
				// // return (this->allocator.max_size());
				return std::numeric_limits<difference_type>::max() / (sizeof(node_type) / 2 ?: 1);
			}

			reference	front(){
				return (this->l_begin->content);
			}

			const_reference	front() const {
				return (this->l_begin->content);
			}

			reference	back(){
				if (this->empty() == true)
					return (this->front());
				return (this->l_end->prev->content);
			}

			const_reference	back() const {
				if (this->empty() == true)
					return (this->front());
				return (this->l_end->prev->content);
			}

			template <class InputIterator>
			void assign (InputIterator first, InputIterator last,
						typename ft::__void_t<typename ft::iterator_traits<InputIterator>::iterator_category>::type* = 0){
				this->clear();
				for (InputIterator i = first ; i != last ; i++){
					this->push_back(*i);
				}
			}

			void	assign (size_t n, const value_type& val){
				this->clear();
				for (size_t i = 0 ; i < n ; i++){
					this->push_back(val);
				}
			}
			
			void	push_front(const value_type& val){
				node<value_type> *tmp = new node<value_type> (val);
				this->l_begin->insert_before(tmp);
				this->l_begin = tmp;
				this->len++;
			}

			void	pop_front (){
				if (this->empty())
					return ;
				node<T>* node = this->l_begin;
				this->l_begin = node->next;
				node->delete_node(node);
				this->len--;
			}

			void	push_back(const value_type& n){
				node<value_type> *tmp = new node<value_type> (n);
				this->l_end->insert_before(tmp);
				if (this->empty()){
					this->l_begin = tmp;
				}
				this->len++;
			}

			void	pop_back(){
				if (this->empty())
					return ;
				node<T>* node = this->l_end;
				this->l_end = node->prev;
				this->l_end->prev->next = this->l_end;
				node->delete_node(node);
				this->len--;
			}

			// single element
			iterator	insert (iterator position, const value_type& val){
				if (position == this->begin()){
					this->push_front(val);
					return (this->begin());
				}
				else if (position == this->end()){
					this->push_back(val);
					return (this->l_end->prev);
				}
				else {
					node<value_type> *tmp = new node<value_type> (val);
					position.get_node()->insert_before(tmp);
					this->len++;
					return (position.get_node()->prev);
				}
			}
			// fill
			void	insert (iterator position, size_type n, const value_type& val){
				for (size_t i = 0 ; i < n ; i++){
					this->insert(position, val);
				}

			}
			// range
			// void	insert (iterator position, iterator first, iterator last){
			template <class InputIterator>
			void insert (iterator position, InputIterator first, InputIterator last,
							typename ft::__void_t<typename ft::iterator_traits<InputIterator>::iterator_category>::type* = 0){
				while ( first != last ){
					insert(position, *first);
					first++;
				}
			}

			// erase(){
			iterator erase (iterator position){
				iterator tmp = position;
				tmp++;
				if (position == this->begin())
					this->l_begin = position.get_node()->next;
				if (position == this->end())
					return position;
				position.get_node()->prev->next = position.get_node()->next;
				position.get_node()->next->prev = position.get_node()->prev;
				delete position.get_node();
				this->len--;
				return (tmp);
			}
			// range
			iterator erase (iterator first, iterator last){
				while ( first != last ){
					first = erase(first);
				}
				return (first);
			}

			void	swap(list &x){
				node_pointer	tmp;
				size_t			l_tmp;

				tmp = this->l_begin;
				this->l_begin = x.l_begin;
				x.l_begin = tmp;

				tmp = this->l_end;
				this->l_end = x.l_end;
				x.l_end = tmp;

				l_tmp = this->len;
				this->len = x.len;
				x.len = l_tmp;
			}

			void resize (size_t n){
				iterator tmp;
				tmp = this->l_begin;
				if (n < this->len){
					for (size_t i = 0 ; i < n ; i++){
						tmp++;
					}
					this->erase(tmp, this->end());
				} else if ( n > this->len){
					this->insert(this->end(), n - this->len, 0);
				}
			}

			void resize (size_t n, value_type val){
				iterator tmp;
				tmp = this->l_begin;
				if (n < this->len){
					for (size_t i = 0 ; i < n ; i++){
						tmp++;
					}
					this->erase(tmp, this->end());
				} else if ( n > this->len){
					this->insert(this->end(), n - this->len, val);
				}
			}

			void clear(){
				iterator first = begin();
				iterator last = end();
				erase(first, last);
			}

			// entire list (1)	
			void splice (iterator position, list& x){
				this->splice(position, x, x.begin(), x.end());
			}
			// single element (2)	
			void splice (iterator position, list& x, iterator i){
				iterator i2 = i;
				i2++;
				this->splice(position, x, i, i2);
			}
			// element range (3)
			void splice (iterator position, list& x, iterator first, iterator last){

				iterator tmp = first;
				tmp++;
				if (position == this->begin())
					this->l_begin = first.get_node();
				while (first != last){
					first.get_node()->prev->next = first.get_node()->next;
					first.get_node()->next->prev = first.get_node()->prev;
					// on sort first de sa liste
					first.get_node()->prev = position.get_node()->prev;
					// on set le prev de first : l'ancien prev de pos
					first.get_node()->prev->next = first.get_node();
					// on set le next du nouveau prev de first : first
					first.get_node()->next = position.get_node();
					// on set le next de first : position
					position.get_node()->prev = first.get_node();
					// on set le nouveau prev de position : first
					x.len--;
					if (x.len == 0){
						x.l_begin = x.l_end;
						x.l_end->prev = x.l_begin;
						x.l_begin->next = x.l_end;
					}
					if (this->len == 0){
						this->l_begin = first.get_node();
						this->l_end->prev = first.get_node();
					}
					this->len++;
					first = tmp;
					if (tmp != last)
						tmp++;
				}
			}

			void	remove(const value_type& val){
				iterator tmp = this->begin();
				while (tmp != this->end()){
					if (val == *tmp)
						tmp = erase(tmp);
					else
						tmp++;
				}
			}
			template <class Predicate>
			void	remove_if(Predicate pred){
				iterator tmp = this->begin();
				while (tmp != this->end()){
					if (pred(*tmp))
						tmp = erase(tmp);
					else
						tmp++;
				}
			}

			// removes all but the first element from every consecutive group of equal elements in the container.
			void unique(){
				this->unique(are_equal);
			}

			//  takes as argument a specific comparison function that determine the "uniqueness" of an element.
			// In fact, any behavior can be implemented (and not only an equality comparison),
			// but notice that the function will call binary_pred(*i,*(i-1)) for all pairs of elements
			// (where i is an iterator to an element, starting from the second)
			// and remove i from the list if the predicate returns true.

			template <class BinaryPredicate>
			void unique (BinaryPredicate binary_pred){
				iterator	it = this->begin();
				iterator	it2 = it;
				it2++;
				while (it2 != this->end()) {
					if (binary_pred(*it2, *it))
						it2 = erase(it2);
					else {
						it2++;
						it++;
					}
				}
			}

			void merge(list& x){
				this->merge(x, less_than);
			}

			template <class Compare>
			void merge (list& x, Compare comp){
				if (x != *this){
					iterator it = this->begin();
					iterator itx = x.begin();
					iterator tmp = itx;
					while (x.size() > 0)
					{
						while (it != this->end() && (comp(*it, *itx) || !comp(*itx, *it))){
							it++;
						}
						tmp = itx;
						tmp++;
						this->splice(it, x, itx, tmp);
						itx = tmp;
					}
				}
			}

			// The sorting is performed by applying an algorithm that uses either operator< (in version (1))
			// or comp (in version (2)) to compare elements. 
			// This comparison shall produce a strict weak ordering of the elements
			// (i.e., a consistent transitive comparison, without considering its reflexiveness).
			void	sort(){
				this->sort(less_than);
			}

			// https://www.geeksforgeeks.org/sorting-algorithms/#Basic

			template <class Compare>
			void	sort (Compare comp){
				if (this->size() <= 1)
					return ;
				size_t	i, j; 
				for (i = 0; i < this->len-1; i++){
					for (j = 0; j < this->len-i-1; j++){
						if (comp((this->get_node(j + 1))->content, (this->get_node(j))->content)){
							node_pointer first = this->get_node(j);
							node_pointer second = this->get_node(j + 1);

							first->next = second->next;
							second->next = first;
							second->prev = first->prev;
							first->prev = second;
							second->prev->next = second;
							first->next->prev = first;
							if (first == l_begin)
								l_begin = second;
						}
					}
				}
			}


/*
No contained elements are accessed: concurrently accessing or modifying them is safe, although iterating through the container is not.
*/
			void	reverse(){
					iterator	it = iterator(this->l_end);
					size_type	size = this->len + 1;
					node_type	*tmp;

					while (size != 0){
						tmp = it.get_node()->prev;
						it.get_node()->prev = it.get_node()->next;
						it.get_node()->next = tmp;
						--it;
						--size;
					}
					this->l_begin = it.get_node()->next;
			}
	};

	template <class T, class Alloc>
	bool operator== (const list<T,Alloc>& lhs, const list<T,Alloc>& rhs){
		if (lhs.size() != rhs.size())
			return (false);
		ft::listIterator<ft::node<T>, const T> lhs_it = lhs.begin();
		ft::listIterator<ft::node<T>, const T> rhs_it = rhs.begin();
		while (lhs_it != lhs.end() && rhs_it != rhs.end()) {
			if (*lhs_it != *rhs_it)
				return (false);
			lhs_it++;
			rhs_it++;
		}
		return (true);
	}

	template <class T, class Alloc>
	bool operator!= (const list<T,Alloc>& lhs, const list<T,Alloc>& rhs){
		return (!(lhs == rhs));
	}

	template<class T, class Alloc>
	bool operator<(const list<T, Alloc>& lhs, const list<T, Alloc>& rhs) {
		return ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}

	template <class T, class Alloc>
	bool operator<= (const list<T,Alloc>& lhs, const list<T,Alloc>& rhs){
		return ((lhs < rhs) || (lhs == rhs));
	}

	template <class T, class Alloc>
	bool operator> (const list<T,Alloc>& lhs, const list<T,Alloc>& rhs){
		return (!(lhs <= rhs));
	}

	template <class T, class Alloc>
	bool operator>= (const list<T,Alloc>& lhs, const list<T,Alloc>& rhs){
		return (!(lhs < rhs));
	}

	template <class T, class Alloc>
	void swap (list<T,Alloc>& x, list<T,Alloc>& y){
		x.swap(y);
	}
}
