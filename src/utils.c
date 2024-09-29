/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 14:00:32 by hluiz-ma          #+#    #+#             */
/*   Updated: 2024/09/29 16:25:10 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

void	error_handle(void)
{
		perror("Error");
		exit(EXIT_FAILURE);
}

int	open_file(char *filename, int mode)
{
	int	fd;

	if (mode == 1)
		fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	else
		fd = open(filename, O_RDONLY, 0777);
	if (fd == -1)
	{
		perror(ERR_FILE);
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
/*
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
}*/

char	*path_cmd(char *cmd, char *env[])
{
	int		i;
	char	*exec;
	char	**env_path;
	char	*path_part;

	if (ft_strncmp(cmd, "/", 1) == 0 || ft_strncmp(cmd, "./", 2) == 0 || ft_strncmp(cmd, "../", 3) == 0)
	{
		if (access(cmd, F_OK | X_OK) == 0)
			return (ft_strdup(cmd));
		else
		{
			perror(ERR_CMD);
			return (NULL);
		}
	}
	i = 0;
	while(!ft_strnstr(env[++i], "PATH=", 5))
		i++;
	env_path = ft_split(env[i] + 5, ':');
	i = -1;
	while (env_path[++i])
	{
		path_part = ft_strjoin(env_path[i], "/");
		exec = ft_strjoin(path_part, cmd);
		free(path_part);
		if (access(exec, F_OK | X_OK) == 0)
		{
			ft_free_matx(env_path);
			return (exec);
		}
		free(exec);
	}
	ft_free_matx(env_path);
	perror(ERR_CMD);
	return (NULL);
}
bool	has_spaces(char *cmd)
{
	int i = 0;

	if (!cmd)
		return false;
	while (cmd[i])
	{
		if (cmd[0] == ' ')
			return true;
		i++;
	}
	return false;
}

int	is_special_command(char *cmd)
{
	if (ft_strncmp(cmd, "ls", 2) == 0 || ft_strncmp(cmd, "cd", 2) == 0 ||
		ft_strncmp(cmd, "pwd", 3) == 0 || ft_strncmp(cmd, "echo", 4) == 0)
	{
		return (1); // Retorna verdadeiro se for um comando especial
	}
	return (0);
}