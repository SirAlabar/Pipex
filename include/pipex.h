/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 19:02:24 by hluiz-ma          #+#    #+#             */
/*   Updated: 2024/09/18 19:58:11 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

/*
**                              HEADERS
*/
# include "../libft/libft.h"
# include <stdbool.h> // for bool
# include <stdlib.h> //malloc,free, exit + rand
# include <stdio.h>//perror
# include <string.h> //strerror
# include <unistd.h> //read, write
# include <sys/wait.h> //getid
# include <fcntl.h>

# define RESET "\033[0m"
# define GREEN "\033[0;32m"
# define RED "\033[0;31m"
/*
**                              STRUCTS
*/





/*
**                              PROTOTYPES
*/

void	parent_process(int *fd);


#endif