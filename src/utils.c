/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/11 20:00:33 by decortejohn       #+#    #+#             */
/*   Updated: 2024/12/28 12:30:30 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

static void	write_heredoc(int fd, char *limiter, t_pipe *pipex)
{
	char	*line;
	size_t	len;

	len = ft_strlen(limiter);
	while (1)
	{
		write(1, "heredoc> ", 9);
		line = get_next_line(0, pipex->limiter);
		if (!line)
		{
			get_next_line(-1, pipex->limiter);
			return ;
		}
		if (!ft_strncmp(line, limiter, len) && line[len] == '\n')
		{
			free(line);
			get_next_line(-1, pipex->limiter);
			return ;
		}
		write(fd, line, ft_strlen(line));
		free(line);
	}
}

void	free_paths(char **paths)
{
	int	i;

	i = -1;
	while (paths[++i])
		free(paths[i]);
	free(paths);
}

void	handle_heredoc(t_pipe *pipex, char *limiter)
{
	pipex->infile = open(".heredoc_tmp", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	write_heredoc(pipex->infile, limiter, pipex);
	close(pipex->infile);
	pipex->infile = open(".heredoc_tmp", O_RDONLY);
	if (pipex->infile < 0)
		error_exit(ERR_FILE);
}

void	create_pipes(t_pipe *pipex)
{
	int	i;

	pipex->pipes = malloc(sizeof(int *) * (pipex->cmd_count - 1));
	if (!pipex->pipes)
		error_exit(ERR_MALLOC);
	i = -1;
	while (++i < pipex->cmd_count - 1)
	{
		pipex->pipes[i] = malloc(sizeof(int) * 2);
		if (!pipex->pipes[i])
			error_exit(ERR_MALLOC);
		if (pipe(pipex->pipes[i]) < 0)
			error_exit(ERR_PIPE);
	}
}

void	setup_redirects(t_pipe *pipex, int cmd_index)
{
	if (cmd_index == 0)
		dup2(pipex->infile, STDIN_FILENO);
	else
		dup2(pipex->pipes[cmd_index - 1][0], STDIN_FILENO);
	if (cmd_index == pipex->cmd_count - 1)
		dup2(pipex->outfile, STDOUT_FILENO);
	else
		dup2(pipex->pipes[cmd_index][1], STDOUT_FILENO);
	close_all_pipes(pipex);
}
