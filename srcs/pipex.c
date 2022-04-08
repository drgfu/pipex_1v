/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkais <kkais@student.42wolfsburg.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/07 13:26:58 by kkais             #+#    #+#             */
/*   Updated: 2022/04/08 11:52:49 by kkais            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	check_file_access(t_data *data, char **av)
{
	if (access(av[1], F_OK) != 0)
	{
		write(data->dap_out, "No such file or directory\n", 26);
		exit(1);
	}
	if (access(av[1], R_OK) != 0)
	{
		write(data->dap_out,
			"Error occurred while opening the input file!\n", 45);
		exit(1);
	}
	if (access(av[1], R_OK | F_OK) == 0)
		data->pro.file1 = open(av[1], O_RDONLY, 00500);
	if (access(av[4], F_OK) != 0)
	{
		data->pro.file2 = open(av[4], O_CREAT | O_RDWR, 00774);
		if (data->pro.file2 < 0)
		{
			write(data->dap_out,
				"Error occurred while opening the output file!\n", 46);
			exit(1);
		}
	}
	else
		data->pro.file2 = open(av[4], O_RDWR | O_TRUNC, 00774);
}

static void	parent(int fd[2], char **av, char *env[], t_data *data)
{
	char	**str;

	data->pro.path = NULL;
	data->pro.dup_check1 = dup2(fd[0], STDIN_FILENO);
	data->pro.dup_check2 = dup2(data->pro.file2, STDOUT_FILENO);
	if (data->pro.dup_check1 == -1 || data->pro.dup_check2 == -1)
	{
		write(data->dap_out, "Error occured with dup2!\n", 25);
		exit(1);
	}
	close(fd[1]);
	close(fd[0]);
	data->pro.path = get_path(env, av[3], data);
	str = ft_split(av[3], ' ');
	if (execve(data->pro.path, str, env) == -1)
	{
		write(data->dap_out, "Error occured with executing!\n", 30);
		exit(1);
	}
}

static void	child(int fd[2], char **av, char *env[], t_data *data)
{
	char	**str;

	data->pro.path = NULL;
	data->pro.dup_check1 = dup2(fd[1], STDOUT_FILENO);
	data->pro.dup_check2 = dup2(data->pro.file1, STDIN_FILENO);
	if (data->pro.dup_check1 == -1 || data->pro.dup_check2 == -1)
	{
		write(data->dap_out, "Error occured with dup2!\n", 25);
		exit(1);
	}
	close(fd[0]);
	close(fd[1]);
	data->pro.path = get_path(env, av[2], data);
	str = ft_split(av[2], ' ');
	if (execve(data->pro.path, str, env) == -1)
	{
		write(data->dap_out, "Error occured with executing!\n", 30);
		exit(1);
	}
}

static void	creat_pipe(char **av, char *env[], t_data *data)
{
	int	pid;
	int	fd[2];

	if (pipe(fd) == -1)
	{
		write(1, "Error ocurred while opening the pipe\n", 37);
		exit(1);
	}
	check_file_access(data, av);
	pid = fork();
	if (pid == -1)
	{
		write(data->dap_out, "Error occurred while forking\n", 29);
		exit(1);
	}
	if (pid == 0)
		child(fd, av, env, data);
	else
	{
		parent(fd, av, env, data);
		waitpid(pid, NULL, 0);
	}
}

int	main(int ac, char *av[], char *env[])
{
	t_data	data;

	data.dap_out = dup(1);
	if (ac == 5 && env[0] != NULL)
		creat_pipe(av, env, &data);
	else
	{
		write(data.dap_out, "Wrong number of arguments!\n", 27);
		exit(1);
	}
	return (0);
}
