/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   List_node.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/08 12:17:20 by alienard          #+#    #+#             */
/*   Updated: 2021/05/20 20:26:12 by alienard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <string>
#include <limits>
#include "iterator_traits.hpp"

namespace ft {
	template<typename T>
	struct node
	{
			typedef T					value_type;
			typedef value_type			*pointer;
			typedef value_type			&reference;
			typedef node<value_type>	*node_pointer;
			typedef node<value_type>	&node_reference;

			node_pointer	prev;
			value_type		content;
			node_pointer	next;

			node(){
				prev = NULL;
				content = T();
				next = NULL;
			}

			~node(){
				
			}

			node(const T &n){
				prev = NULL;
				content = T(n);
				next = NULL;
			}

			node(const node<T>& node){
				*this = node;
			}

			reference operator*(){return content;}

			node&		operator=(const node<T>& node){
				this->prev = node.prev;
				this->content = node.content;
				this->next = node.next;
				return *this;
			}

			node_reference	self(){
				return &(*this);
			}

			node_pointer	base(){
				return &(*this);
			}

			void insert_before(node<T>* node){
				this->prev->next = node;
				node->prev = this->prev;
				node->next = this;
				this->prev = node;
			}

			void delete_node(node<T>* node){
				node->prev->next = node->next;
				node->next->prev = node->prev;
				delete node;
			}
	};
}
