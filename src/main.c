/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/30 14:09:15 by jdecorte          #+#    #+#             */
/*   Updated: 2024/10/10 21:21:12 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

int main(int argc, char **argv, char **env)
{
	t_pipe pipex;
	int i;

	if(argc < 5)
		error_exit(ERR_ARGS);
	init_pipe(&pipex, argc, argv, env);
	create_pipes(&pipex);
	if(pipex.is_heredoc)
		handle_heredoc(&pipex, pipex.limiter);

	i = -1;
	while(++i < pipex.cmd_count)
		execute_cmd(&pipex, argv[i + 2 + pipex.is_heredoc], i);
	close_all_pipes(&pipex);
	wait_all_processes(&pipex);
	cleanup(&pipex);
	return (0);
}