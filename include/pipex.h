/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 19:02:24 by hluiz-ma          #+#    #+#             */
/*   Updated: 2024/09/22 20:36:50 by hluiz-ma         ###   ########.fr       */
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

# define ERR_PIPE "Error: pipe failed."
# define ERR_FORK "Error: fork failed."
# define ERR_EXECVE "Error: execve failed."
# define ERR_ARGS "Error: usage: ./pipex infile cmd1 cmd2 ... outfile\n"
# define ERR_PERM "Error: permission denied: "
# define ERR_MALC "Error: memory allocation failed: "
# define ERR_CMD "Error: command not found: "
# define ERR_FILE "Error: no such file or directory: "

/*
**                              STRUCTS
*/

typedef struct s_args
{
	char	**args;
	char	*pathname;
	char	*infile;
	char	*outfile;
	int		argc;
	bool	first_child;
	bool	last_child;
}			t_args;

/*
**                              PROTOTYPES
*/

void		parent_process(int *fd);
void		child_process(int *fd, char *cmd, char *env[]);

void		exec_cmd(char *cmd, char **env);

int			open_file(char *filename, int is_output);

void		exec_cmd(char *cmd, char **env);
void		exec_pipes(char *cmd, char *env[]);

char		*path_cmd(char *cmd, char *env[]);
char		*my_env(char *name, char *env[]);

void		ft_free_matx(char **matx);

void	ft_error(int fd, char *error_msg, char *filename);

void init_args(t_args *args, int ac);

#endif