/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 19:02:12 by hluiz-ma          #+#    #+#             */
/*   Updated: 2024/09/18 21:13:54 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

void	parent_process(int *fd)
{
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
	close(fd[1]);
}

int main(int ac , char *av[], char *envp[])
{
    int fd[2];
    pid_t pid;
    pid_t pid2;
    int i;

    if (ac != 5)
        return(ft_printf("Invalid number of arguments!", 1));
    
           

    if (pid == 0)
    {
        dup2(fd[1], STDOUT_FILENO);
        close(fd[0]);
        close(fd[1]);
        
        char *cmd1_args[] = { av[3], av[4], NULL };
        execve(cmd1_args[0], cmd1_args, envp);
        return(perror("Execve error"), 4); 
    }

 //   ./pipex infile cmd1 cmd2 outfile
 //env

./pipex infile "ls -l" "" outifile

// primeir child abre infile 
 //ultima abre outfile

 
    i = 2;
    while(i < ac -1)
    {
        if(pipe(fd) == -1)
            return(ft_printf("Pipe error!"), 2);

        pid = fork();
        if(pid == -1)
            return(ft_printf("Fork error!"), 3);    
        if (pid == 0)
        {
            child
        }
        else 
        {
            parent_process(fd);
        }

        
    }
    if(pid != 0)
        wait(NULL);
    
    return (0);
}



//wait(NULL) para esperar as childrens 
//terminarem os processos antes de fecha
/*
main
    int fd[2];
    pid_t pid;
    
while (enquanto tiver comando < i)
{
    pipe(fd);
    checks do pipe (devolver as msg de erro caso falhe);   
    pid=fork(); 
    if(pid == 0)
    {
        child_process(todo);
    }else
        parent_process(todo);
    
    i++;
    
}
*/