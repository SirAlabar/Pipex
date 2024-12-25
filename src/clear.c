
#include "../include/pipex.h"

void wait_all_processes(t_pipe *pipex)
{
    int i;

    i = -1;
    while (++i < pipex->cmd_count)
        waitpid(pipex->pids[i], NULL, 0);
    if (pipex->is_heredoc)
        unlink(".heredoc_tmp");
}

void error_exit(char *msg)
{
   if (msg)
       perror(msg);
   exit(EXIT_FAILURE);
}

void close_all_pipes(t_pipe *pipex)
{
    int i;

    i = -1;
    while(++i < pipex->cmd_count -1)
    {
        close(pipex->pipes[i][0]);
        close(pipex->pipes[i][1]);        
    }
    if(pipex->infile > 0)
        close(pipex->infile);
    if(pipex->outfile > 0)
        close(pipex->outfile);
}

void free_pipes_and_pids(t_pipe *pipex)
{
   int i;

    if (pipex && pipex->pids)
   {
       free(pipex->pids);
       pipex->pids = NULL;
   }
    if (pipex && pipex->pipes)
   {
       i = -1;
       while (++i < pipex->cmd_count - 1 && pipex->pipes[i])
       {
           free(pipex->pipes[i]);
           pipex->pipes[i] = NULL;
       }
       free(pipex->pipes);
       pipex->pipes = NULL;
   }
}

void cleanup(t_pipe *pipex)
{
    int i;

    if (!pipex)
        return;
   if (pipex->cmd_args)
   {
       i = -1;
       while (pipex->cmd_args[++i])
       {
           free(pipex->cmd_args[i]);
           pipex->cmd_args[i] = NULL;
       }
       free(pipex->cmd_args);
       pipex->cmd_args = NULL;
   }
   if (pipex->cmd_path)
   {
       free(pipex->cmd_path);
       pipex->cmd_path = NULL;
   }
   free_pipes_and_pids(pipex);
}
