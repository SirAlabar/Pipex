/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 19:02:12 by hluiz-ma          #+#    #+#             */
/*   Updated: 2024/09/22 20:31:48 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

void	parent_process(int *fd)
{
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
	close(fd[1]);
}

void	child_process(int *fd, char *cmd, char *env[])
{
	close(fd[0]);
	dup2(fd[1], STDOUT_FILENO);
	exec_cmd(cmd, env);
}

void	exec_cmd(char *cmd, char **env)
{
	char	**args;
	char	*path;

	args = ft_split(cmd, ' ');
	path = path_cmd(args[0], env);
	if (execve(path, args, env) == -1)
	{
		ft_putstr_fd("Command not found: ", 2);
		ft_putendl_fd(args[0], 2);
		ft_free_matx(args);
		exit(0);
	}
}

int	main(int ac, char *av[], char *envp[])
{
	int	i;
	int	fd[2];
	pid_t pid;
	t_args *args;

	if (ac != 5)
		return (ft_error(2, ERR_ARGS, NULL), 2);
	args =
	i = 2;
	while (i < ac - 1)
	{
		if(pipe(fd) == -1)
			return(perror(ERR_PIPE), 1);
		pid = fork();
		if (pid == -1)
			return(perror(ERR_FORK), 1);
		if (pid == 0)
			child_process();
		else
		 	parent_process();
		i++;
	}
	if (pid != 0)
		wait(NULL);
	FREE();
	return (0);
}
