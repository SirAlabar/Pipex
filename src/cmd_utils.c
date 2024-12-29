/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 12:23:55 by hluiz-ma          #+#    #+#             */
/*   Updated: 2024/12/28 12:31:41 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

static char	*try_paths(char **paths, char *cmd)
{
	char	*part_path;
	char	*path;
	char	*result;
	int		i;

	result = NULL;
	i = -1;
	while (paths[++i])
	{
		part_path = ft_strjoin(paths[i], "/");
		path = ft_strjoin(part_path, cmd);
		free(part_path);
		if (access(path, F_OK) == 0)
		{
			result = path;
			break ;
		}
		free(path);
	}
	return (result);
}

char	*find_path(char *cmd, char **env)
{
	char	**paths;
	char	*path;
	char	*result;

	if (access(cmd, F_OK) == 0)
		return (ft_strdup(cmd));
	path = get_env_path(env);
	if (!path)
		return (NULL);
	paths = ft_split(path, ':');
	if (!paths)
		return (NULL);
	result = try_paths(paths, cmd);
	free_paths(paths);
	return (result);
}

char	**get_cmd_args(char *cmd)
{
	char	**args;

	args = ft_split(cmd, ' ');
	if (!args)
		return (NULL);
	return (args);
}

char	*get_env_path(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], "PATH=", 5) == 0)
			return (env[i] + 5);
		i++;
	}
	return (NULL);
}

void	execute_cmd(t_pipe *pipex, char *cmd, int cmd_index)
{
	pipex->pids[cmd_index] = fork();
	if (pipex->pids[cmd_index] < 0)
		error_exit(ERR_FORK);
	if (pipex->pids[cmd_index] == 0)
	{
		setup_redirects(pipex, cmd_index);
		pipex->cmd_args = get_cmd_args(cmd);
		if (!pipex->cmd_args)
		{
			cleanup(pipex);
			error_exit(ERR_MALLOC);
		}
		pipex->cmd_path = find_path(pipex->cmd_args[0], pipex->env);
		if (!pipex->cmd_path)
		{
			cleanup(pipex);
			error_exit(ERR_CMD);
		}
		execve(pipex->cmd_path, pipex->cmd_args, pipex->env);
		cleanup(pipex);
		error_exit(ERR_EXECVE);
	}
}
