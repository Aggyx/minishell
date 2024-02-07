/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smagniny <smagniny@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 19:43:49 by smagniny          #+#    #+#             */
/*   Updated: 2024/02/08 00:43:06 by smagniny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header.h"

int g_status = 0;

static	void	init_ms(t_var *var, const char	**envp)
{
	var->envp = NULL;
	var->inputline = NULL;
	cpy_env(&var->envp, envp);
    updt_shlvl(&var->envp);
    ms_get_capabilities();
    ms_signal();
}

static void	init_values(t_var *var)
{
	var->fd_in = 0;
	var->fd_out = 0;
    var->tokens = NULL;
    var->nb_node = 0;
}

int	main(int argc, char **argv, const char **envp)
{
	t_var	var;
    int     last_status;

	init_ms(&var, envp);
	while (get_inputline(&var))
	{
        init_values(&var);
		if (lexer(&var))
        {
            ft_lstclear_node(&var.tokens);
            continue ;
        }
        else if (argc > 1 && !ft_strncmp(argv[1], "print", 6))
        {
            printNodes(&var.tokens);
            ft_lstclear_node(&var.tokens);
            continue;
        }
        while (var.tokens)
        {
            if (run_builtin_parent(&var, &last_status) == IS_NOT_BUILTIN)
                last_status = process_handler(&var);
            else

                var.tokens = ft_lstdelone_node_getnext(var.tokens);
        }
		free(var.tokens);
        g_status = last_status;
	}
	ft_freeenv(&var.envp);
	return (0);
}
