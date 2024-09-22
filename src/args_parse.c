/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_parse.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 20:08:16 by hluiz-ma          #+#    #+#             */
/*   Updated: 2024/09/22 20:37:18 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

void init_args(t_args *args, int ac)
{
    int i;

    i = 0;
    while(i <= ac - 3)
    {
        args[i].args = NULL;
        args[i].pathname = NULL;
        args[i].first_child = false;
        args[i].last_child = false;
        args[i].infile = NULL;
        args[i].outfile = NULL;
        args[i].argc = 0;
        i++;
    }
}


t_args *arg_parse(int ac, char *av[], char *envp[])
{
    t_args *args;
    int i;

    args = (t_args *)malloc(sizeof(t_args) * (ac - 2));
    if (!args)
        return(perror(ERR_MALC), NULL);
    init_args(args, ac);
    i = 0;
    while(i < ac - 3)
    {
        
        
    }
    return (args);    
}