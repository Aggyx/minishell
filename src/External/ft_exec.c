/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/09 16:01:41 by smagniny          #+#    #+#             */
/*   Updated: 2024/01/25 01:15:04 by math             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header.h"

static	char	**malloc_flagsandparams_node(t_node *node)
{
	t_subnode	*tmp;
	int			size;
	char		**args;


	tmp = (*node).flags;
	size = 0;
	while (tmp)
	{
		size++;
		tmp = tmp->next;
	}
	tmp = (*node).params;
	while (tmp)
	{
		size++;
		tmp = tmp->next;
	}
	args = (char **)malloc((size + 2) * sizeof(char *));
	if (!args)
		return (NULL);
	return (args);
}

static	char	**set_flagsandparams_to_array(t_node *node)
{
	t_subnode	*tmp;
	char		**args;
	int			i;

	args = malloc_flagsandparams_node(node);
	tmp = (*node).flags;
	i = 0;
	args[i++] = ft_strdup(node->token->content);
	while (tmp)
	{
		args[i++] = ft_strdup(tmp->content);
		tmp = tmp->next;
	}
	tmp = (*node).params;
	while (tmp)
	{
		args[i++] = ft_strdup(tmp->content);
		tmp = tmp->next;
	}
	args[i] = NULL;
	return (args);
}

char	**envlist_to_array(t_env *envlist)
{
	char	**envp;
	t_env	*tmp;
	int		count;
	int		i;

	i = 0;
	count = 0;
	tmp = envlist;
	while (tmp != NULL)
	{
		count++;
		tmp = tmp->next;
	}
	envp = (char **)malloc((count + 1) * sizeof(char *));
	tmp = envlist;
	while (i < count)
	{
		envp[i++] = ft_strdup(tmp->line_env);
		tmp = tmp->next;
	}
	envp[i] = NULL;
	return (envp);
}

static	char	**find_path_env(char **envp)
{
	int		i;
	char	**tmp;

	i = 0;
	while (ft_strncmp("PATH=", envp[i], 5) != 0)
		i++;
	tmp = ft_split(envp[i] + 5, ':');
	return (tmp);
}

static	char	*find_path(char **envp, char	*command)
{
	char	*path;
	char	*ppath;
	char	**tmp;
	int		i;

	if (!command || command == NULL)
		return (NULL);
	if (!(ft_strncmp(command, "./", 2) && ft_strncmp(command, "/", 1) && ft_strncmp(command, "../", 3))) //  && access(command, X_OK | F_OK) == 0
		return (command);
	else
	{
		tmp = find_path_env(envp);
		i = -1;
		while (tmp[++i] && command)
		{
			ppath = ft_strjoin(tmp[i], "/");
			path = ft_strjoin(ppath, command);
			free(ppath);
			if (access(path, X_OK | F_OK) == 0)
			{	
				doublefree(tmp);
				return (path);
			}
			free(path);
		}
		doublefree(tmp);
	}
	return (NULL);
}

int		ft_exec(t_var *var)
{
	char	**args;
	char	**envp;
	char	*exec_path;
	
	args = set_flagsandparams_to_array(var->tokens);
	envp = envlist_to_array(var->envp);
	exec_path = find_path(envp, var->tokens->token->content);
	if (exec_path != NULL)
	{
		printf("LAUNCHING EXECVE\n");
		if (execve(exec_path, args, envp) == -1) //exit process
		{
			perror("execve");
			free(exec_path);
			doublefree(envp);
			doublefree(args);
			exit(127);
		}
	}
	printf("Minishell: Command not found: %s\n", \
		var->tokens->token->content);
	free(exec_path);
	doublefree(envp);
	doublefree(args);
	return(EXIT_FAILURE);
}
