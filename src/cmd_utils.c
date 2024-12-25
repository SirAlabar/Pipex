
#include "../include/pipex.h"

char *find_path(char *cmd, char **env)
{
    char    **paths;
    char    *path;
    char    *part_path;
    int     i;

    if (access(cmd, F_OK) == 0)
        return (ft_strdup(cmd));
    path = get_env_path(env);
    paths = ft_split(path, ':');
    i = 0;
    while (paths[i])
    {
        part_path = ft_strjoin(paths[i], "/");
        path = ft_strjoin(part_path, cmd);
        free(part_path);
        if (access(path, F_OK) == 0)
            return (path);
        free(path);
        i++;
    }
    return (NULL);
}

char **get_cmd_args(char *cmd)
{
    char **args;
    
    args = ft_split(cmd, ' ');
    if (!args)
        return (NULL);
    return (args);
}

char *get_env_path(char **env)
{
    int i;

    i = 0;
    while (env[i])
    {
        if (ft_strncmp(env[i], "PATH=", 5) == 0)
            return (env[i] + 5);
        i++;
    }
    return (NULL);
}