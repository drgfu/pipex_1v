/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkais <kkais@student.42wolfsburg.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/07 13:31:25 by kkais             #+#    #+#             */
/*   Updated: 2022/04/08 12:59:15 by kkais            ###   ########.fr       */
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
	while (data->path_split[data->i])
	{
		data->ptr = ft_strjoin(data->path_split[data->i], "/");
		data->final_path = ft_strjoin(data->ptr, data->cmd);
		free(data->ptr);
		data->check_access = access(data->final_path, X_OK | F_OK);
		if (data->check_access == 0)
			break ;
		else
			free(data->final_path);
		data->i++;
	}
	if (data->check_access != 0)
	{
		write(data->dap_out, "Command not found!\n", 19);
		exit(1);
	}
	free_split(data->path_split);
	if (ft_strchr(av, ' ') != 0)
		free(data->cmd);
	return (data->final_path);
}

void	check_cmd(t_data *data, char *av)
{
	if (ft_strchr(av, ' ') != 0)
	{
		data->cmd_split = ft_split(av, ' ');
		if (data->cmd_split[0] == NULL)
		{
			write(data->dap_out, "Command not found!\n", 19);
			exit(1);
		}
		data->cmd = ft_strdup(data->cmd_split[0]);
		free_split(data->cmd_split);
	}
	else
		data->cmd = av;
}

char	*get_path(char **env, char *av, t_data *data)
{
	data->i = 0;
	data->path = NULL;
	data->cmd_split = NULL;
	check_cmd(data, av);
	while (env[data->i])
	{
		if (ft_strnstr(env[data->i], "PATH", 4) != NULL)
		{
			data->path = ft_strtrim(env[data->i], "PATH=");
			break ;
		}
		data->i++;
	}
	if (data->path == NULL)
	{
		write(1, "Path not found!\n", 16);
		exit(1);
	}
	data->path_split = ft_split(data->path, ':');
	free(data->path);
	data->i = 0;
	return (found_path(data, av));
}
