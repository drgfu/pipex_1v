/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkais <kkais@student.42wolfsburg.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/07 13:31:25 by kkais             #+#    #+#             */
/*   Updated: 2022/04/07 16:44:14 by kkais            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	free_split(char **ptr)
{
	int	i;

	i = 0;
	while (ptr[i] != NULL)
	{
		free(ptr[i]);
		i++;
	}
	free(ptr);
}

char	*found_path(t_data *data, char *av)
{
	// for every path in the array
	while (data->path_split[data->i])
	{
		// add / to the path and then add this str to ptr
		data->ptr = ft_strjoin(data->path_split[data->i], "/");
		// then add the command that we want to find in the path and add the whole path to final_path
		data->final_path = ft_strjoin(data->ptr, data->cmd);
		// free ptr because we need to use it with every path
		free(data->ptr);
		// checks if we have access to the path
		data->check_access = access(data->final_path, X_OK | F_OK);
		// if access return 0, the access is given, so it's the right path and we stop the while loop
		if (data->check_access == 0)
			break ;
		// otherwise access is not given, so we free the final_path
		else
			free(data->final_path);
		data->i++;
	}
	// if we can't find accessable path, error
	if (data->check_access != 0)
	{
		write(data->dap_out, "Can't find the path\n", 20);
		exit(1);
	}
	free_split(data->path_split);
	// if there's more than one command, free commands
	if (ft_strchr(av, ' ') != 0)
		free(data->cmd);
	// return the final path
	return (data->final_path);
}

void	check_cmd(t_data *data, char *av)
{
	// if there's a space, so 2 commands
	if (ft_strchr(av, ' ') != 0)
	{
		// split the arguments at the space and add them to data struct
		data->cmd_split = ft_split(av, ' ');
		// if there are no commands, error
		if (data->cmd_split[0] == NULL)
		{
			write(data->dap_out, "Command not found!\n", 18);
			exit(1);
		}
		// add the first command to the current command in data
		data->cmd = ft_strdup(data->cmd_split[0]);
		// free split for memory
		free_split(data->cmd_split);
	}
	// otherwise there's one command, so the arguments becomes the command
	else
		data->cmd = av;
}

// finds the path and commands
// PATH in env gives search path for commands
char	*get_path(char **env, char *av, t_data *data)
{
	// initialize, so that I can use them
	data->i = 0;
	data->path = NULL;
	data->cmd_split = NULL;
	// find the commands from the arguments and add them to the data struct
	check_cmd(data, av);
	// is going through environmental values to find the path
	while (env[data->i])
	{
		// when it finds a match for path in env
		if (ft_strnstr(env[data->i], "PATH", 4) != NULL)
		{
			// adds the path to the data
			data->path = ft_strtrim(env[data->i], "PATH=");
			break ;
		}
		data->i++;
	}
	// if it doesn't find the path, error
	if (data->path == NULL)
	{
		write(1, "path not found!\n", 16);
		exit(1);
	}
	// splits path at colon, because these are different paths
	data->path_split = ft_split(data->path, ':');
	free(data->path);
	data->i = 0;
	// return the final path to the command
	return (found_path(data, av));
}
