/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkais <kkais@student.42wolfsburg.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/07 13:26:58 by kkais             #+#    #+#             */
/*   Updated: 2022/04/07 17:59:36 by kkais            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

// check the access to the filein 
static void	check_file_access(t_data *data, char **av)
{
	// access checks if the user has permission to access the file
	// the first argument is the path, so the filename that the user wants to access
	// the second arguments is how (int) and indicates the access mode
	if (access(av[1], F_OK) != 0)
	{
		write(data->dap_out, "No such file or directory\n", 26);
		exit(1);
	}
	if (access(av[1], R_OK) != 0)
	{
		write(data->dap_out, "Error occurred while opening the input file!\n", 45);
		exit(1);
	}
	if (access(av[1], R_OK | F_OK) == 0)
		data->pro.file1 = open(av[1], O_RDONLY, 00500);
	// checks if the output file exists
	if (access(av[4], F_OK) != 0)
	{
		data->pro.file2 = open(av[4], O_CREAT | O_RDWR, 00774);
		if (data->pro.file2 < 0)
		{
			write(data->dap_out, "Error occurred while opening the output file!\n", 46);
			exit(1);
		}
	}
	else
		data->pro.file2 = open(av[4], O_RDWR | O_TRUNC, 00774);
}

static void	parent(int fd[2], char **av, char *env[], t_data *data)
{
	char	**str;

	// set the process path to null as we have a new process
	data->pro.path = NULL;
	// set the read end of the pipe as input
	data->pro.dup_check1 = dup2(fd[0], STDIN_FILENO);
	// set the outfile as output
	data->pro.dup_check2 = dup2(data->pro.file2, STDOUT_FILENO);
	// check for failure
	if (data->pro.dup_check1 == -1 || data->pro.dup_check2 == -1)
	{
		write(data->dap_out, "Error occured with dup2!\n", 25);
		exit(1);
	}
	// close files for memory leaks
	close(fd[1]);
	close(fd[0]);
	// get path for the second commands
	data->pro.path = get_path(env, av[3], data);
	// split commands at the space
	str = ft_split(av[3], ' ');
	// execute commands
	execve(data->pro.path, str, env);
}

static void	child(int fd[2], char **av, char *env[], t_data *data)
{
	char	**str;

	data->pro.path = NULL;
	// dup2 - like dup, creates a copy of a fd but dup2 can swap our fds to stdin/stdout
	// sets our write end as the stdout
	data->pro.dup_check1 = dup2(fd[1], STDOUT_FILENO);
	// sets our infile file to stdinput
	data->pro.dup_check2 = dup2(data->pro.file1, STDIN_FILENO);
	// if any file fails, error message
	if (data->pro.dup_check1 == -1 || data->pro.dup_check2 == -1)
	{
		write(data->dap_out, "Error occured with dup2!\n", 25);
		exit(1);
	}
	// close files for memory leaks
	close(fd[0]);
	close(fd[1]);
	// finds the path to the command ands adds ot to the process
	data->pro.path = get_path(env, av[2], data);
	// split arguments at the space
	str = ft_split(av[2], ' ');
	// execve - execution of the program that is referred to by pathname
	// we have the path to the commands and then just execute the commands in the str array
	execve(data->pro.path, str, env);
}

// takes given variables, environmental variable and data struct as paramenters
static void	creat_pipe(char **av, char *env[], t_data *data)
{
	//process id
	int	pid;
	// there are 2 fds needed for piping
	int	fd[2];
	// error management if piping fails
	if (pipe(fd) == -1)
	{
		write(1, "Error ocurred while opening the pipe\n", 37);
		exit(1);
	}
	// checks if infile and outfile and accessable
	check_file_access(data, av);
	// forks the process and returns an int (successful, fail, etc)
	pid = fork();
	if (pid == -1)
	{
		write(data->dap_out, "Error occurred while forking\n", 29);
		exit(1);
	}
	// id in the child process is always 0
	if (pid == 0)
		child(fd, av, env, data);
	// if there's an actual process id, it's the main/parent process
	else
	{
		parent(fd, av, env, data);
		// wait for the child process to finish
		waitpid(pid, NULL, 0);
	}
}

// we get arguments and environmental variable
// environmental variables define the behavior of the environment
int	main(int ac, char *av[], char *env[])
{
	t_data	data;
	//creates a copy of a fd 1 which is the stdout fd in linux to the data struct
	data.dap_out = dup(1);
	//checks if the arguments are correct, environmental variable shouldn't be empty
	if (ac == 5 && env[0] != NULL)
		//goes into creating a pipe
		creat_pipe(av, env, &data);
	// otherwise writes error into output
	else
	{
		write(data.dap_out, "Wrong number of arguments!\n", 27);
		exit(1);
	}
	return (0);
}
