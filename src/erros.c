/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   erros.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 19:38:44 by hluiz-ma          #+#    #+#             */
/*   Updated: 2024/09/22 19:44:59 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

void	ft_error(int fd, char *error_msg, char *filename)
{
	if (error_msg != NULL)
		ft_putstr_fd(error_msg, 2);
	if (filename != NULL)
		ft_putstr_fd(filename, 2);
	write(fd, "\n", 1);
}