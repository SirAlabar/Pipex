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

void	exec_cmd(char *cmd, char **env)
{
	char	**args;
	char	*path;

	if (!cmd || !*cmd || cmd[0] == ' ')
		error_handle();
	args = ft_split(cmd, ' ');
	path = path_cmd(args[0], env);
	if (!path)
	{
		ft_free_matx(args);
		free(path);
		exit(127);
	}
	if (execve(path, args, env) == -1)
	{
		ft_free_matx(args);
		free(path);
		error_handle();
	}
}

void	child_process(char *av[], int *fd, char *env[])
{
	int	c_fd;

	if (!av[2] || !*av[2])
	{
		ft_putstr_fd(ERR_CMD, 2);
		exit(127);
	}
	c_fd = open(av[1], O_RDONLY, 0777);
	if (c_fd == -1)
		error_handle();
	dup2(fd[1], STDOUT_FILENO);
	dup2(c_fd, STDIN_FILENO);
	close(fd[0]);
	close(fd[1]);
	close(c_fd);
	exec_cmd(av[2], env);
}

void	parent_process(char *av[], int *fd, char *env[])
{
	int	p_fd;

	if (!av[3] || !*av[3])
	{
		ft_putstr_fd(ERR_CMD, 2);
		exit(127);
	}
	if (access(av[4], F_OK) == -1)
	{
		perror(av[4]);
		exit(127);
	}
	p_fd = open(av[4], O_WRONLY | O_CREAT | O_TRUNC, 0777);
	dup2(fd[0], STDIN_FILENO);
	dup2(p_fd, STDOUT_FILENO);
	close(fd[1]);
	close(fd[0]);
	close(p_fd);
	exec_cmd(av[3], env);
}

int	main(int ac, char **av, char **env)
{
	int		p_fd[2];
	pid_t	pid;

	if (ac == 5)
	{
		if (pipe(p_fd) == -1)
			exit(-1);
		pid = fork();
		if (pid == -1)
			exit(-1);
		if (pid == 0)
			child_process(av, p_fd, env);
		waitpid(pid, NULL, 0);
		parent_process(av, p_fd, env);
	}
	else
	{
		ft_putstr_fd("\033[31mError: Bad arguments\n\e[0m", 2);
		ft_putstr_fd("Ex: ./pipex <file1> <cmd1> <cmd2> <file2>\n", 1);
	}
	return (0);
}
