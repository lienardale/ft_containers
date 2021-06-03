/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Map.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard@student.42.fr <alienard>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/13 13:46:56 by alienard          #+#    #+#             */
/*   Updated: 2021/05/21 13:31:51 by alienard@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <string>
#include <limits>
#include "Map_node.hpp"
#include "Map_iterators.hpp"
#include "algorithm.hpp"
#include "iterator_traits.hpp"

// https://en.wikipedia.org/wiki/Red%E2%80%93black_tree
// https://en.cppreference.com/w/cpp/container/map/map
// https://en.cppreference.com/w/cpp/container/map

namespace ft
{
	template<class Key,class T,class Compare = ft::less<Key>,class Allocator = std::allocator<ft::pair<const Key, T> > >
	class map{
		public :
			typedef Key											key_type;
			typedef T											mapped_type;
			typedef ft::pair< const key_type, mapped_type>		value_type;
			typedef const value_type							const_value_type;

			typedef ft::node< value_type >						node_type;
			typedef node_type									*node_ptr;

			typedef Compare										key_compare;

			typedef Allocator									allocator_type;
			typedef typename allocator_type::reference			reference;
			typedef typename allocator_type::const_reference	const_reference;
			typedef typename allocator_type::pointer			pointer;
			typedef typename allocator_type::const_pointer		const_pointer;

			typedef mapIterator<node_type, value_type>			iterator;
			typedef mapIterator<node_type, const_value_type>	const_iterator;
			typedef mapreverseIterator<iterator>				reverse_iterator;
			typedef mapreverseIterator<const_iterator>			const_reverse_iterator;

			typedef std::ptrdiff_t					difference_type;
			typedef std::size_t						size_type;


			// from https://www.cplusplus.com/reference/map/map/value_comp/
			class value_compare
			{
				friend class map;
				protected:
					Compare comp;
					value_compare (Compare c) : comp(c) {}  // constructed with map's comparison object
				public:
					bool operator() (const value_type& x, const value_type& y) const
					{
						return comp(x.first, y.first);
					}
			};

		private :
			key_compare		comp;
			size_type		_size;
			allocator_type	alloc;
			node_type		*root;
			node_type		*last;

// HELPER FCTS TO BUILD AND USE RED AND BLACK TREE
		protected :
			void					map_init()
			{
				this->_size = 0;
				this->root = &this->_end;
			}

			// initializes the nodes with appropirate values
			// all the pointers are set to point to the null pointer
			void initializeNULLNode(node_ptr node, node_ptr parent) {
				node->data = 0;
				node->parent = parent;
				node->left = NULL;
				node->right = NULL;
				node->color = 0;
			}
			
			void preOrderHelper(node_ptr node) {
				if (node != last) {
					std::cout << node->data<<" ";
					preOrderHelper(node->left);
					preOrderHelper(node->right);
				} 
			}

			void inOrderHelper(node_ptr node) {
				if (node != last) {
					inOrderHelper(node->left);
					std::cout << node->data<<" ";
					inOrderHelper(node->right);
				} 
			}

			void postOrderHelper(node_ptr node) {
				if (node != last) {
					postOrderHelper(node->left);
					postOrderHelper(node->right);
					std::cout << node->data<<" ";
				} 
			}

			node_ptr searchTreeHelper(node_ptr node, key_type key) {
				if (node == last || key == node->data) {
					return node;
				}

				if (key < node->data) {
					return searchTreeHelper(node->left, key);
				} 
				return searchTreeHelper(node->right, key);
			}

			// fix the rb tree modified by the delete operation
			void fixDelete(node_ptr x) {
				node_ptr s;
				while (x != root && x->color == BLACK) {
					if (x == x->parent->left) {
						s = x->parent->right;
						if (s->color == RED) {
							// case 3.1
							s->color = BLACK;
							x->parent->color = RED;
							leftRotate(x->parent);
							s = x->parent->right;
						}

						if (s->left->color == BLACK && s->right->color == BLACK) {
							// case 3.2
							s->color = RED;
							x = x->parent;
						} else {
							if (s->right->color == BLACK) {
								// case 3.3
								s->left->color = BLACK;
								s->color = RED;
								rightRotate(s);
								s = x->parent->right;
							} 

							// case 3.4
							s->color = x->parent->color;
							x->parent->color = BLACK;
							s->right->color = BLACK;
							leftRotate(x->parent);
							x = root;
						}
					} else {
						s = x->parent->left;
						if (s->color == RED) {
							// case 3.1
							s->color = BLACK;
							x->parent->color = RED;
							rightRotate(x->parent);
							s = x->parent->left;
						}

						if (s->right->color == BLACK && s->right->color == BLACK) {
							// case 3.2
							s->color = RED;
							x = x->parent;
						} else {
							if (s->left->color == BLACK) {
								// case 3.3
								s->right->color = BLACK;
								s->color = RED;
								leftRotate(s);
								s = x->parent->left;
							} 

							// case 3.4
							s->color = x->parent->color;
							x->parent->color = BLACK;
							s->left->color = BLACK;
							rightRotate(x->parent);
							x = root;
						}
					} 
				}
				x->color = BLACK;
			}


			void rbTransplant(node_ptr u, node_ptr v){
				if (u->parent == NULL) {
					root = v;
				} else if (u == u->parent->left){
					u->parent->left = v;
				} else {
					u->parent->right = v;
				}
				v->parent = u->parent;
			}

			size_t deleteNodeHelper(node_ptr node, Key key) {
				// find the node containing key
				node_ptr z = last;
				node_ptr x, y;
				while (node != last){
					if (node->data.first == key) {
						z = node;
					}

					if (comp(node->data.first, key)) {
						node = node->right;
					} else {
						node = node->left;
					}
				}

				if (z == last) {
					// std::cout << "Couldn't find key in the tree"<< std::endl;
					return 0;
				} 

				y = z;
				int y_original_color = y->color;
				if (z->left == last) {
					x = z->right;
					rbTransplant(z, z->right);
				} else if (z->right == last) {
					x = z->left;
					rbTransplant(z, z->left);
				} else {
					y = minimum(z->right);
					y_original_color = y->color;
					x = y->right;
					if (y->parent == z) {
						x->parent = y;
					} else {
						rbTransplant(y, y->right);
						y->right = z->right;
						y->right->parent = y;
					}

					rbTransplant(z, y);
					y->left = z->left;
					y->left->parent = y;
					y->color = z->color;
				}
				delete z;
				this->_size--;
				if (y_original_color == 0){
					fixDelete(x);
				}
				return 1;
			}
			
			// fix the red-black tree
			void fixInsert(node_type *k){
				node_type *u;
				while (k->parent->color == RED) {
					if (k->parent == k->parent->parent->right) {
						u = k->parent->parent->left; // uncle
						if (u->color == RED) {
							// case 3.1
							u->color = BLACK;
							k->parent->color = BLACK;
							k->parent->parent->color = RED;
							k = k->parent->parent;
						} else {
							if (k == k->parent->left) {
								// case 3.2.2
								k = k->parent;
								rightRotate(k);
							}
							// case 3.2.1
							k->parent->color = BLACK;
							k->parent->parent->color = RED;
							leftRotate(k->parent->parent);
						}
					} else {
						u = k->parent->parent->right; // uncle

						if (u->color == RED) {
							// mirror case 3.1
							u->color = BLACK;
							k->parent->color = BLACK;
							k->parent->parent->color = RED;
							k = k->parent->parent;	
						} else {
							if (k == k->parent->right) {
								// mirror case 3.2.2
								k = k->parent;
								leftRotate(k);
							}
							// mirror case 3.2.1
							k->parent->color = BLACK;
							k->parent->parent->color = RED;
							rightRotate(k->parent->parent);
						}
					}
					if (k == root) {
						break;
					}
				}
				root->color = BLACK;
			}

			void printHelper(node_type *root, std::string indent, bool end) {
				// print the tree structure on the screen
				if (root != last) {
					std::cout << indent;
					if (end) {
						std::cout << "R----";
						indent += "     ";
					} else {
						std::cout << "L----";
						indent += "|    ";
					}
					std::string sColor = root->color?"RED":"BLACK";
					std::cout << root->data.first << ":"<< root->data.second << "(" << sColor << ")"<< std::endl;
					printHelper(root->left, indent, false);
					printHelper(root->right, indent, true);
				}
				// cout<<root->left->data<<endl;
			}

			// Pre-Order traversal
			// Node->Left Subtree->Right Subtree
			void preorder() {
				preOrderHelper(this->root);
			}

			// In-Order traversal
			// Left Subtree -> Node -> Right Subtree
			void inorder() {
				inOrderHelper(this->root);
			}

			// Post-Order traversal
			// Left Subtree -> Right Subtree -> Node
			void postorder() {
				postOrderHelper(this->root);
			}

			// search the tree for the key k
			// and return the corresponding node
			node_ptr searchTree(int k) {
				return searchTreeHelper(this->root, k);
			}

			// find the node with the minimum key
			node_type *minimum(node_type *node) {
				if (_size < 1)
					return node;
				while (node->left != last) {
					node = node->left;
				}
				return node;
			}

			// find the node with the minimum key
			node_type *minimum(node_type *node) const {
				if (_size < 1)
					return node;
				while (node->left != last) {
					node = node->left;
				}
				return node;
			}

			// find the node with the maximum key
			node_type * maximum(node_type * node) {
				while (node != last) {
					node = node->right;
				}
				return node;
			}

			// find the node with the maximum key
			node_type * maximum(node_type * node) const {
				while (node != last) {
					node = node->right;
				}
				return node;
			}

			// find the successor of a given node
			node_type *successor(node_type * x) {
				// if the right subtree is not null,
				// the successor is the leftmost node in the
				// right subtree
				if (!x)
					return last;
				if (x->right != last) {
					return minimum(x->right);
				}

				// else it is the lowest ancestor of x whose
				// left child is also an ancestor of x.
				node_type * y = x->parent;
				while (y != NULL && x == y->right) {
					x = y;
					y = y->parent;
				}
				return y;
			}

			// find the predecessor of a given node
			node_ptr predecessor(node_ptr x) {
				// if the left subtree is not null,
				// the predecessor is the rightmost node in the 
				// left subtree
				if (x->left != last) {
					return maximum(x->left);
				}

				node_ptr y = x->parent;
				while (y != last && x == y->left) {
					x = y;
					y = y->parent;
				}
				return y;
			}

			// rotate left at node x
			void leftRotate(node_type *x) {
				node_type * y = x->right;
				x->right = y->left;
				if (y->left != last) {
					y->left->parent = x;
				}
				y->parent = x->parent;
				if (x->parent == NULL) {
					this->root = y;
				} else if (x == x->parent->left) {
					x->parent->left = y;
				} else {
					x->parent->right = y;
				}
				y->left = x;
				x->parent = y;
			}

			// rotate right at node x
			void rightRotate(node_type *x) {
				node_type *y = x->left;
				x->left = y->right;
				if (y->right != last) {
					y->right->parent = x;
				}
				y->parent = x->parent;
				if (x->parent == NULL) {
					this->root = y;
				} else if (x == x->parent->right) {
					x->parent->right = y;
				} else {
					x->parent->left = y;
				}
				y->right = x;
				x->parent = y;
			}

			node_ptr getRoot(){
				return this->root;
			}

			node_ptr getLast(){
				return this->last;
			}

			// delete the node from the tree
			size_t deleteNode(Key data) {
				return deleteNodeHelper(this->root, data);
			}

			// print the tree structure on the screen
			void prettyPrint() {
				if (root) {
					printHelper(this->root, "", true);
				}
			}
		public:

// MAP FUNCTIONS

			// default constructor
			explicit map (const key_compare& comp = key_compare(),
							const allocator_type& alloc = allocator_type()){
				this->comp = comp;
				this->alloc = alloc;
				this->_size = 0;
				this->last = new node_type;
				this->last->color = BLACK;
				this->last->left = NULL;
				this->last->right = NULL;
				this->root = this->last;
				this->last->parent = this->root;

			}
			virtual ~map(){
				this->clear();
				delete last;
			}
			// range (2)	
			template <class InputIterator>
			map (InputIterator root, InputIterator last,
					const key_compare& comp = key_compare(),
					const allocator_type& alloc = allocator_type()){
				this->comp = comp;
				this->alloc = alloc;
				this->_size = 0;
				this->last = new node_type ();
				this->last->color = BLACK;
				this->last->left = NULL;
				this->last->right = NULL;
				this->root = this->last;
				this->last->parent = this->root;

				this->insert(root, last);
			}
			// copy (3)	
			map (const map& x){
				this->comp = x.comp;
				this->alloc = x.alloc;
				this->_size = 0;
				this->last = new node_type ();
				this->last->color = BLACK;
				this->last->left = NULL;
				this->last->right = NULL;
				this->last->parent = this->root;
				this->root = this->last;
				this->insert(x.begin(), x.end());
			}
			// assignation operator
			map &operator= (const map &x){
				this->clear();
				if (last)
					delete last;
				this->comp = x.comp;
				this->alloc = x.alloc;
				this->_size = 0;
				this->last = new node_type ();
				this->last->color = BLACK;
				this->last->left = NULL;
				this->last->right = NULL;
				this->last->parent = this->root;
				this->root = this->last;
				this->insert(x.begin(), x.end());
				return (*this);
			}

			template<class U>
			value_compare& value_comp(U i, U j)
				{return i == j;}

			template<class U>
			const value_compare& value_comp(U i, U j) const
				{return i == j;}

/*
https://www.cplusplus.com/reference/map/map/operator[]/
A call to this function is equivalent to:
(*((this->insert(make_pair(k,mapped_type()))).first)).second
*/
			mapped_type& operator[] (const key_type& k){
				pair<iterator, bool> pib = this->insert(value_type(k, mapped_type()));
				mapped_type& ret = pib.first->second;
				return (ret);
			}

/*
Because element keys in a map are unique,
the insertion operation checks whether each inserted element has a key equivalent to the one of an element already in the container,
and if so, the element is not inserted, returning an iterator to this existing element (if the function returns a value).
*/
			// single element (1)	
			pair<iterator,bool> insert (const value_type& val){
				iterator it = this->find(val.first);
				if (it != this->end()){
					return pair<iterator, bool> (it, false);
				}
				else {
					it = rbt_insert(val);
					return pair<iterator, bool> (it, true);
				}
			}
			// with hint (2)	
			iterator insert (iterator position, const value_type& val){
				static_cast<void>(position);
				return this->insert(val).first;
			}
			// range (3)		
			template <class InputIterator>
			void insert (InputIterator f, InputIterator l){
				for( ; f != l ; f++){
					insert(*f);
				}
			}

		protected:
			// insert the key to the tree in its appropriate position
			// and fix the tree
			iterator rbt_insert(const value_type& key) {
				// Ordinary Binary Search Insertion
				node_ptr node = new node_type (key);
				node->left = last;
				node->right = last;
				node->color = RED; // new node must be red

				node_ptr y = NULL;
				node_ptr x = this->root;

				while (x != last) {
					y = x;
					if (comp(node->data.first, x->data.first)) {
						x = x->left;
					} else {
						x = x->right;
					}
				}
				// y is parent of x
				node->parent = y;
				if (y == NULL) {
					root = node;
				} else if (comp(node->data.first, y->data.first)) {
					y->left = node;
				} else {
					y->right = node;
				}
				// if new node is a root node, simply return
				if (node->parent == NULL){
					node->color = BLACK;
					this->_size++;
					return iterator(node);
				}
				// if the grandparent is null, simply return
				if (node->parent->parent == NULL) {
					this->_size++;
					return iterator(node);
				}
				// Fix the tree
				fixInsert(node);
				this->_size++;
				this->last->parent = this->root;
				return iterator(node);
			}

		public:
			iterator	begin(){
				return (iterator(this->minimum(this->root)));
			}

			const_iterator begin() const {
				return (const_iterator(this->minimum(this->root)));
			}
			
			iterator	end(){
				return (iterator(this->last));
			}
			
			const_iterator	end() const {
				return (const_iterator(this->last));
			}

			// reverse iterators
			reverse_iterator rbegin(){
				return (reverse_iterator(this->end()));
			}

			const_reverse_iterator rbegin() const{
				return (const_reverse_iterator(this->end()));
			}

			reverse_iterator rend(){
				return (reverse_iterator(this->begin()));
			}

			const_reverse_iterator rend() const{
				return (const_reverse_iterator(this->begin()));
			}

			bool empty() const{
				return (_size <= 0);
			}

			size_type size() const{
				return this->_size;
			}

			size_type max_size() const{
				// return std::min<size_type>(
				// 	__node_traits::max_size(__node_alloc()),
				// 	numeric_limits<difference_type >::max());
				// return std::numeric_limits<difference_type>::max();
				// return (this->alloc.max_size());
				// return std::numeric_limits<difference_type>::max() / (sizeof(node_type) / 2.4 ?: 1);
				return std::numeric_limits<difference_type>::max() / (sizeof(node_type) / 2 ?: 1);
				// return std::numeric_limits<difference_type>::max() / 20;
			}

			size_type count (const key_type& k) const{
				if (this->find(k) != this->end())
					return 1;
				return 0;
			}

			void clear(){
				erase(this->begin(), this->end());
				this->_size = 0;
			}

			// (1)	position
			void erase (iterator position){
				node_ptr x, y, z;

				z = position.get_n();
				y = z;
				int y_original_color = y->color;
				if (z->left == last) {
					x = z->right;
					rbTransplant(z, z->right);
				} else if (z->right == last) {
					x = z->left;
					rbTransplant(z, z->left);
				} else {
					y = minimum(z->right);
					y_original_color = y->color;
					x = y->right;
					if (y->parent == z) {
						x->parent = y;
					} else {
						rbTransplant(y, y->right);
						y->right = z->right;
						y->right->parent = y;
					}
					rbTransplant(z, y);
					y->left = z->left;
					y->left->parent = y;
					y->color = z->color;
				}
				delete z;
				if (y_original_color == 0){
					fixDelete(x);
				}
				this->_size--;
			}
			// (2)	key
			size_type erase (const key_type& k){
				return this->deleteNode(k);
			}
			// (3)	range
			void erase (iterator f, iterator l){
				iterator it;
				while ( f != l ) {
					it = f;
					it++;
					erase(f);
					f = it;
				}
			}

			ft::pair<iterator,iterator> equal_range( const Key& key ){
				return ft::pair<iterator, iterator> (lower_bound(key), upper_bound(key));
			}

			ft::pair<const_iterator,const_iterator> equal_range( const Key& key ) const{
				return ft::pair<const_iterator, const_iterator> (lower_bound(key), upper_bound(key));
			}

			iterator find (const key_type& k){
				iterator it = begin();
				while (it != end()){
					if (!comp(it->first, k) && !comp(k, it->first))
						return it;
					it++;
				}
				return it;
			}

			const_iterator	find(const key_type& k) const{
				const_iterator it = begin();
				while (it != end()){
					if (it->first == k)
						return it;
					it++;
				}
				return it;
			}

			iterator lower_bound( const Key& key ){
				iterator it = begin();
				while ( it != end() ){
					if (it->first >= key)
						return it;
					it++;
				}
				return end();
			}

			const_iterator lower_bound( const Key& key ) const{
				const_iterator it = begin();
				while ( it != end() ){
					if (it->first >= key)
						return it;
					it++;
				}
				return end();
			}

			iterator upper_bound( const Key& key ){
				iterator it = begin();
				while ( it != end() ){
					if (it->first > key)
						return it;
					it++;
				}
				return end();
			}

			const_iterator upper_bound( const Key& key ) const{
				const_iterator it = begin();
				while ( it != end() ){
					if (it->first > key)
						return it;
					it++;
				}
				return end();
			}

			void swap (map& x){
				key_compare		comp_tmp;
				size_type		_size_tmp;
				allocator_type	alloc_tmp;
				node_type		*root_tmp;
				node_type		*last_tmp;

				comp_tmp = this->comp;
				_size_tmp = this->_size;
				alloc_tmp = this->alloc;
				root_tmp = this->root;
				last_tmp = this->last;

				this->comp = x.comp;
				this->_size = x._size;
				this->alloc = x.alloc;
				this->root = x.root;
				this->last = x.last;

				x.comp = comp_tmp;
				x._size = _size_tmp;
				x.alloc = alloc_tmp;
				x.root = root_tmp;
				x.last = last_tmp;
			}

			key_compare key_comp() const{
				return comp;
			}

			value_compare value_comp() const{
				return value_compare(comp);
			}

	};

	template <class Key, class T, class Compare, class Alloc>
	void swap (map<Key,T,Compare,Alloc>& x, map<Key,T,Compare,Alloc>& y){
		x.swap(y);
	}

/*
The equality comparison (operator==) is performed by first comparing sizes, and if they match, the elements are compared sequentially using operator==, stopping at the first mismatch (as if using algorithm equal).

The less-than comparison (operator<) behaves as if using algorithm lexicographical_compare, which compares the elements sequentially using operator< in a reciprocal manner (i.e., checking both a<b and b<a) and stopping at the first occurrence.

The other operations also use the operators == and < internally to compare the elements, behaving as if the following equivalent operations were performed:
operation	equivalent operation
a!=b	!(a==b)
a>b		b<a
a<=b	!(b<a)
a>=b	!(a<b)

Notice that none of these operations take into consideration the internal comparison object of either container, but compare the elements (of type value_type) directly.

value_type is a pair type, and as such, by default, two elements will compare equal only if both their key and mapped value compare equal, and one compare lower than the other only if the first key is lower, or if the keys are equivalent and the mapped value is lower.
*/

	template <class Key, class T, class Compare, class Alloc>
	bool operator== ( const map<Key,T,Compare,Alloc>& lhs,
						const map<Key,T,Compare,Alloc>& rhs ){
		if (lhs.size() != rhs.size())
			return false ;
		ft::mapIterator< ft::node< ft::pair<const Key, T> >, const ft::pair<const Key, T> > itl = lhs.begin();
		ft::mapIterator< ft::node< ft::pair<const Key, T> >, const ft::pair<const Key, T> > itr = rhs.begin();
		while (itl != lhs.end()){
			if (*itl != *itr)
				return false;
			itl++;
			itr++;
		}
		return true;
	}

	template <class Key, class T, class Compare, class Alloc>
	bool operator!= ( const map<Key,T,Compare,Alloc>& lhs,
						const map<Key,T,Compare,Alloc>& rhs ){
		return !(lhs==rhs);
	}

	template <class Key, class T, class Compare, class Alloc>
	bool operator<  ( const map<Key,T,Compare,Alloc>& lhs,
						const map<Key,T,Compare,Alloc>& rhs ){
		return ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}

	template <class Key, class T, class Compare, class Alloc>
	bool operator<= ( const map<Key,T,Compare,Alloc>& lhs,
						const map<Key,T,Compare,Alloc>& rhs ){
		return !(rhs<lhs);
	}

	template <class Key, class T, class Compare, class Alloc>
	bool operator>  ( const map<Key,T,Compare,Alloc>& lhs,
						const map<Key,T,Compare,Alloc>& rhs ){
		return rhs<lhs;
	}

	template <class Key, class T, class Compare, class Alloc>
	bool operator>= ( const map<Key,T,Compare,Alloc>& lhs,
						const map<Key,T,Compare,Alloc>& rhs ){
		return !(lhs<rhs);
	}

// fcts below are in pseudo-code, see https://en.wikipedia.org/wiki/Red%E2%80%93black_tree

/*	
RBnode* joinRightRB(RBnode* TL, k, RBnode* TR){

		if (r(TL)=⌊r(TL)/2⌋×2)
			return Node(TL,⟨k,RED,TR);
		else 
			(L1,⟨k1,c1⟩,R1)=expose(TL);
			T1=Node(L1,⟨k1,c1⟩,joinRightRB(R1,k,TR);
			if (c1=black) and (T1.right.color=T1.right.right.color=red):
				T1.right.right.color=black;
				return rotateLeft(T1);
			else return T1;
	}

	function joinLeftRB(TL, k, TR){

	// symmetric to joinRightRB
	}
	function join(TL, k, TR){
		if ⌊r(TL)/2⌋>⌊r(TR)/2⌋×2:
			T1=joinRightRB(TL,k,TR)
			if (T1.color=red) and (T1.right.color=red):
				T1.color=black;
			return T1;
		else if ⌊r(TR)/2⌋>⌊r(TL)/2⌋×2
			// symmetric
		else if (TL.color=black) and (TR.color=black)
			Node(TL,⟨k,red⟩,TR);
		else
			Node(TL,⟨k,black⟩,TR);
	}

	function split(T, k){

		if (T = nil) return (nil, false, nil)
		(L,(m,c),R) = expose(T);
		if (k = m) return (L, true, R)
		if (k < m) 
			(L1,b,R1) = split(L, k);
			return (L1,b,join(R1,m,R));
		if (k>m) 
			(L1,b,R1) = split(R, k)
			return (join(L,m,L1),b,R));
	}

	function union(t1, t2):
		if t1 = nil:
			return t2
		if t2 = nil:
			return t1
		t<, t> ← split t2 on t1.root
		return join(t1.root, union(left(t1), t<), union(right(t1), t>))

	bulkInsert(T, I, k):
		I.sort()
		bulklInsertRec(T, I, k)

	bulkInsertRec(T, I, k):
		if k = 1:
			forall e in I: T.insert(e)
		else
			m := ⌊size(I) / 2⌋
			(T1, _, T2) := split(T, I[m])
			bulkInsertRec(T1, I[0 .. m], ⌈k / 2⌉)
				|| bulkInsertRec(T2, I[m + 1 .. size(I) - 1], ⌊k / 2⌋)
			T ← join2(T1, T2)
*/

}
