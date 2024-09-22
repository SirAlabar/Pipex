/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 14:00:32 by hluiz-ma          #+#    #+#             */
/*   Updated: 2024/09/22 19:34:56 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

int	open_file(char *filename, int mode)
{
	int	fd;

	if (mode == 1)
		fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0777);
	else if (mode == 2)
		fd = open(filename, O_CREAT | O_WRONLY | O_APPEND, 0777);
	else
		fd = open(filename, O_RDONLY, 0777);
	if (fd == -1)
	{
		perror("Error opening file.");
		exit(1);
	}
	return (fd);
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
	char	**fullpath;
	char	*path_part;
	char	**s_cmd;

	i = -1;
	fullpath = ft_split(my_env("PATH", env), ':');
	s_cmd = ft_split(cmd, ' ');
	while (fullpath[++i])
	{
		path_part = ft_strjoin(fullpath[i], "/");
		exec = ft_strjoin(path_part, s_cmd[0]);
		free(path_part);
		if (access(exec, F_OK | X_OK) == 0)
		{
			ft_free_matx(s_cmd);
			return (exec);
		}
		free(exec);
	}
	ft_free_matx(fullpath);
	ft_free_matx(s_cmd);
	return (cmd);
}
