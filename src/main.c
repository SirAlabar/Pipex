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

static void	handle_files(t_pipe *pipex, int argc, char **argv)
{
	if (pipex->is_heredoc)
	{
		pipex->limiter = argv[2];
		pipex->outfile = open(argv[argc - 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
	}
	else
	{
		if (access(argv[1], F_OK) != 0)
			error_exit(ERR_FILE);
		if (access(argv[1], R_OK) != 0)
			error_exit(ERR_PERM);
		pipex->infile = open(argv[1], O_RDONLY);
		if (pipex->infile < 0)
			error_exit(ERR_PERM);
		pipex->outfile = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (pipex->outfile < 0)
		{
			if (access(argv[argc - 1], F_OK) != 0)
				error_exit(ERR_FILE);
			error_exit(ERR_PERM);
		}
	}
	if (pipex->outfile < 0)
		error_exit(ERR_FILE);
}

void	init_pipe(t_pipe *pipex, int argc, char **argv, char **env)
{
	pipex->env = env;
	pipex->is_heredoc = ft_strncmp(argv[1], "here_doc", 8) == 0;
	pipex->cmd_count = argc - 3 - pipex->is_heredoc;
	pipex->cmd_path = NULL;
	pipex->cmd_args = NULL;
	pipex->pipes = NULL;
	pipex->pids = NULL;
	pipex->infile = 0;
	pipex->outfile = 0;
	pipex->limiter = NULL;
	handle_files(pipex, argc, argv);
	pipex->pids = malloc(sizeof(pid_t) * pipex->cmd_count);
	if (!pipex->pids)
		error_exit(ERR_MALLOC);
}

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