/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/11 20:00:33 by decortejohn       #+#    #+#             */
/*   Updated: 2024/10/10 21:04:58 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

void init_pipe(t_pipe *pipex, int argc, char **argv, char **env)
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
   pipex->pids = malloc(sizeof(pid_t) * pipex->cmd_count);
   if (!pipex->pids)
       error_exit(ERR_MALLOC);
}

static void write_heredoc(int fd, char *limiter, t_pipe *pipex)
{
    char    *line;
    size_t  len;

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


void    handle_heredoc(t_pipe *pipex, char *limiter)
{
   pipex->infile = open(".heredoc_tmp", O_CREAT | O_WRONLY | O_TRUNC, 0644);
   write_heredoc(pipex->infile, limiter, pipex);
   close(pipex->infile);
   pipex->infile = open(".heredoc_tmp", O_RDONLY);
   if (pipex->infile < 0)
       error_exit(ERR_FILE);
}

void create_pipes(t_pipe *pipex)
{
    int i;

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

void setup_redirects(t_pipe *pipex, int cmd_index)
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

void execute_cmd(t_pipe *pipex, char *cmd, int cmd_index)
{
   pipex->pids[cmd_index] = fork();
   if (pipex->pids[cmd_index] < 0)
       error_exit(ERR_FORK);
   if (pipex->pids[cmd_index] == 0)
   {
       setup_redirects(pipex, cmd_index);
       pipex->cmd_args = get_cmd_args(cmd);
       if (!pipex->cmd_args)
           error_exit(ERR_MALLOC);
       pipex->cmd_path = find_path(pipex->cmd_args[0], pipex->env);
       if (!pipex->cmd_path)
           error_exit(ERR_CMD);
       execve(pipex->cmd_path, pipex->cmd_args, pipex->env);
   }
}
