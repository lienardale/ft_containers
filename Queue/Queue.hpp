/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Queue.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/08 11:54:57 by alienard          #+#    #+#             */
/*   Updated: 2021/05/20 20:35:25 by alienard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "List/List.hpp"

namespace ft
{
	template<class T, class Container = list <T> >
	class queue
	{
		public:
			typedef T							value_type;
			typedef Container					container_type;
			typedef std::size_t					size_type;
		private:
			container_type	_container;
		public:
			explicit queue (const container_type& ctnr = container_type()){
				_container = ctnr;
			}

			bool empty() const{
				return _container.empty();
			}
			size_type size() const{
				return _container.size();
			}
			value_type& front(){
				return _container.front();
			}
			const value_type& front() const{
				return _container.front();
			}
			value_type& back(){
				return _container.back();
			}
			const value_type& back() const{
				return _container.back();
			}
			void push (const value_type& val){
				_container.push_back(val);
			}
			void pop(){
				_container.pop_front();
			}
			friend
			bool operator== (const queue<T,Container>& lhs, const queue<T,Container>& rhs){
				return lhs._container == rhs._container;
			}
			friend
			bool operator< (const queue<T,Container>& lhs, const queue<T,Container>& rhs){
				return lhs._container < rhs._container;
			}
	};

	template <class T, class Container>
	bool operator!= (const queue<T,Container>& lhs, const queue<T,Container>& rhs){
		return !(lhs == rhs);
	}

	template <class T, class Container>
	bool operator<= (const queue<T,Container>& lhs, const queue<T,Container>& rhs){
		return !(rhs < lhs);
	}

	template <class T, class Container>
	bool operator>(const queue<T,Container>& lhs, const queue<T,Container>& rhs){
		return (rhs < lhs);
	}

	template <class T, class Container>
	bool operator>= (const queue<T,Container>& lhs, const queue<T,Container>& rhs){
		return !(lhs < rhs);
	}
}
