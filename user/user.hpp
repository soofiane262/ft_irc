/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-mars <sel-mars@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 14:54:36 by sel-mars          #+#    #+#             */
/*   Updated: 2023/03/09 17:14:28 by sel-mars         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string> // string

namespace irc {
	class user {
	  public:
		int			_fd;
		std::string _nickname;
		user( void ) {}
	};
} // namespace irc
