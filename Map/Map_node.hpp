/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Map_node.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard@student.42.fr <alienard>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/21 17:50:29 by alienard          #+#    #+#             */
/*   Updated: 2021/05/21 13:32:09 by alienard@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <string>
#include <limits>
#include "utility.hpp"

namespace ft {

	#define NIL   NULL // null pointer  or  pointer to sentinel node
	#define LEFT  0
	#define RIGHT 1
	#define BLACK	0
	#define RED		1

	template<class T>
	struct node { // node of red–black tree
			int							color; //bool or int depending if on Mac or linux
			int							_unused;
			T							data;
			node*						parent; // == NULL if root of the tree
			node*						right;
			node*						left;

			~node(){
				
			}

			node(const T& n = T()) : data(n){
				parent = NULL;
				right = NULL;
				left = NULL;
			}

			node(const node<T>& n){
				*this = n;
			}

			node&		operator=(const node<T>& n){
				this->data = n.data;
				this->parent = n.parent;
				this->right = n.right;
				this->left = n.left;
				return *this;
			}

			T&	getData(){
				return this->data;
			}

			const T&	getData() const{
				return this->data;
			}

			// find the node with the minimum key
			node *minimum(node *node){
				while (node && node->left != NULL && node->left->data.first) {
					node = node->left;
				}
				return node;
			}

			// find the node with the maximum key
			node * maximum(node * node){
				while (node && node->right != NULL && node->right->data.first) {
					node = node->right;
				}
				return node;
			}

			// find the node with the minimum key
			node *minimum(node *node) const{
				while (node && node->left != NULL && node->left->data.first) {
					node = node->left;
				}
				return node;
			}

			// find the node with the maximum key
			node * maximum(node * node) const{
				while (node && node->right != NULL && node->right->data.first) {
					node = node->right;
				}
				return node;
			}

			node	*getNext() const{
				// if the right subtree is not null,
				// the successor is the leftmost node in the
				// right subtree
				if (this->right != NULL && this->right->data.first) {
					node * y = minimum(this->right);
					return y;
				}
				// else it is the lowest ancestor of x whose
				// left child is also an ancestor of x.
				const node * x = this;
				node * y;
				y = x->parent;
				while ( y != NULL && x == y->right) {
					x = y;
					y = y->parent;
				}
				if (y == NULL){
					y = maximum(this->right);
				}
				return y;
			}

			node	*getPrev() const{
				// if the left subtree is not null,
				// the predecessor is the rightmost node in the 
				// left subtree
				if (this->left != NULL && this->left->data.first) {
					return maximum(this->left);
				}
				const node *x = this;
				node * y = x->parent;
				while (y != NULL && x == y->left) {
					x = y;
					y = y->parent;
				}
				if (!x->data.first && !x->right && !x->left){
					y = this->parent;
					while (y && y->right != NULL && y->right->data.first) {
						y = y->right;
					}
				}
				if (y == NULL){
					y = this->parent;
					while (y->parent)
						y = y->parent;
					while (y->right->data.first)
						y = y->right;
				}
				return y;
			}
	};

}
