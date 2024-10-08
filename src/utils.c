/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/11 20:00:33 by decortejohn       #+#    #+#             */
/*   Updated: 2024/09/29 17:49:35 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

/*
void	exit_handler(int n_exit)
{
	if (n_exit == 1)
		ft_putstr_fd("./pipex infile cmd cmd outfile\n", 2);
	exit(0);
}*/
void	error_handle(void)
{
	perror("Error");
	exit(EXIT_FAILURE);
}

int	open_file(char *file, int in_or_out)
{
	int	ret;

	if (in_or_out == 0)
		ret = open(file, O_RDONLY, 0777);
	if (in_or_out == 1)
		ret = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (ret == -1)
		exit(0);
	return (ret);
}

void	ft_free_matx(char **matx)
{
	int	i;

	i = -1;
	while (matx[++i])
	{
		free(matx[i]);
	}
	free(matx);
}

char	*my_env(char *name, char *env[])
{
	int		i;
	int		j;
	char	*sub;

	i = -1;
	while (env[++i])
	{
		j = -1;
		while (env[i][++j] && env[i][j] != '=')
			j++;
		sub = ft_substr(env[i], 0, j);
		if (ft_strncmp(sub, name, j) == 0)
		{
			free(sub);
			return (env[i] + j + 1);
		}
		free(sub);
	}
	return (NULL);
}

char	*path_cmd(char *cmd, char *env[])
{
	int		i;
	char	*exec;
	char	**envpath;
	char	*path_part;
	char	**s_cmd;

	i = -1;
	envpath = ft_split(my_env("PATH", env), ':');
	s_cmd = ft_split(cmd, ' ');
	while (envpath[++i])
	{
		path_part = ft_strjoin(envpath[i], "/");
		exec = ft_strjoin(path_part, s_cmd[0]);
		free(path_part);
		if (access(exec, F_OK | X_OK) == 0)
		{
			ft_free_matx(s_cmd);
			return (exec);
		}
		free(exec);
	}
	ft_free_matx(envpath);
	ft_free_matx(s_cmd);
	return (cmd);
}
