/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 19:02:12 by hluiz-ma          #+#    #+#             */
/*   Updated: 2024/09/29 13:45:20 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

void	parent_process(char *av[], int *fd, char *env[])
{
	int p_fd;
	
	if (!av[3] || !*av[3])
    {
		ft_putstr_fd(ERR_CMD, 2);
        exit(127);
    }
	p_fd = open_file(av[4], 1);
	dup2(fd[0], STDIN_FILENO);
	dup2(p_fd, STDOUT_FILENO);
	close(fd[1]);
	close(fd[0]);
	exec_cmd(av[3], env);
}

void	child_process(char *av[], int *fd, char *env[])
{
	int c_fd;
	
	if (!av[2] || !*av[2])
    {
		ft_putstr_fd(ERR_CMD, 2);
        exit(127);
    }
	c_fd = open_file(av[1], 0);
	dup2(fd[1], STDOUT_FILENO);
	if (!is_special_command(av[2]))
		dup2(c_fd, STDIN_FILENO);
	close(fd[0]);
	close(fd[1]);
	exec_cmd(av[2], env);
}

void	exec_cmd(char *cmd, char **env)
{
	char	**args;
	char	*path;

	if (!cmd || !*cmd || has_spaces(cmd))
		error_handle ();
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
		error_handle();
		ft_free_matx(args);
		free(path);
		exit(EXIT_FAILURE);
	}
}
void	check_envp(char **envp)
{
	int	i;
	int	check;

	i = -1;
	check = 0;
	while (envp[++i])
		if (ft_strnstr(envp[i], "PATH=", 5) && envp[i][6])
			check = 1;
	if (!check)
	{
		perror("Error \n The environment PATH has no values.");
		exit(1);
	}	
}
int	main(int ac, char *av[], char *envp[])
{
	int fd[2];
	pid_t pid;

	check_envp(envp);
	if (ac == 5)
	{
		if (pipe(fd) == -1)
			exit(-1);
		pid = fork();
		if (pid == -1)
			exit(-1);
		if (pid == 0)
			child_process(av, fd, envp);
		waitpid(pid, NULL, 0);
		parent_process(av, fd, envp);
	}
	else 
	{
		perror("Error \n Wrong number of arguments");
		perror("Usage: ./pipex infile cmd1 cmd2 ... outfile");
		exit(1);
	}
}
