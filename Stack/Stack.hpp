/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Stack.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/08 11:59:15 by alienard          #+#    #+#             */
/*   Updated: 2021/05/21 11:25:32 by alienard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "List/List.hpp"

namespace ft
{
	template<class T, class Container = list <T> >
	class stack
	{
		public:
			typedef T							value_type;
			typedef Container					container_type;
			typedef std::size_t					size_type;
		private:
			container_type	_container;
		public:
			explicit stack (const container_type& ctnr = container_type()){
				_container = ctnr;
			}
			bool empty() const{
				return _container.empty();
			}
			size_type size() const{
				return _container.size();
			}
			value_type& top(){
				return _container.back();
			}
			const value_type& top() const{
				return _container.back();
			}
			void push (const value_type& val){
				return _container.push_back(val);
			}
			void pop(){
				return _container.pop_back();
			}
			friend
			bool operator== (const stack<T,Container>& lhs, const stack<T,Container>& rhs){
				return lhs._container == rhs._container;
			}
			friend
			bool operator< (const stack<T,Container>& lhs, const stack<T,Container>& rhs){
				return lhs._container < rhs._container;
			}
	};

	template <class T, class Container>
	bool operator!= (const stack<T,Container>& lhs, const stack<T,Container>& rhs){
		return !(lhs == rhs);
	}

	template <class T, class Container>
	bool operator<= (const stack<T,Container>& lhs, const stack<T,Container>& rhs){
		return !(rhs < lhs);
	}

	template <class T, class Container>
	bool operator>(const stack<T,Container>& lhs, const stack<T,Container>& rhs){
		return (rhs < lhs);
	}

	template <class T, class Container>
	bool operator>= (const stack<T,Container>& lhs, const stack<T,Container>& rhs){
		return !(lhs < rhs);
	}
}
