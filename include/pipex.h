/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 19:02:24 by hluiz-ma          #+#    #+#             */
/*   Updated: 2024/09/29 17:51:02 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

/*
**                              HEADERS
*/
# include "../libft/libft.h"
# include <fcntl.h>
# include <stdbool.h>  // for bool
# include <stdio.h>    //perror
# include <stdlib.h>   //malloc,free, exit + rand
# include <string.h>   //strerror
# include <sys/wait.h> //getid
# include <unistd.h>   //read, write

# define RESET "\033[0m"
# define GREEN "\033[0;32m"
# define RED "\033[0;31m"

# define ERR_NOCMD 127    // Command not found
# define ERR_NOEXEC 126   // Command exists but not executable
# define ERR_NOPERM 1     // Permission denied
# define ERR_NOFILE 1     // File not found

# define ERR_PIPE "Error: pipe failed."
# define ERR_FORK "Error: fork failed."
# define ERR_EXECVE "Error: execve failed."
# define ERR_ARGS "Error: usage: ./pipex infile cmd1 cmd2 ... outfile"
# define ERR_PERM "Permission denied "
# define ERR_MALLOC "Error: memory allocation failed "
# define ERR_CMD "Command not found "
# define ERR_FILE "No such file or directory "

typedef struct s_pipe
{
    int     infile;
    int     outfile;
    char    *cmd_path;
    char    **cmd_args;
    char    **env;
    int     **pipes;
    pid_t   *pids;
    int     cmd_count;
    int     is_heredoc;
    char    *limiter;
} t_pipe;

/*
**                              PROTOTYPES
*/

//utils
void    init_pipe(t_pipe *pipex, int argc, char **argv, char **env);
void    execute_cmd(t_pipe *pipex, char *cmd, int cmd_index);
void    handle_heredoc(t_pipe *pipex, char *limiter);
void    setup_redirects(t_pipe *pipex, int cmd_index);
void    create_pipes(t_pipe *pipex);

//cmd_utils
char *find_path(char *cmd, char **env);
char **get_cmd_args(char *cmd);
char *get_env_path(char **env);

//clear
void    close_all_pipes(t_pipe *pipex);
void    wait_all_processes(t_pipe *pipex);
void    error_exit(char *msg);
void    free_pipes_and_pids(t_pipe *pipex);
void    cleanup(t_pipe *pipex);

#endif