/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 17:33:24 by smagniny          #+#    #+#             */
/*   Updated: 2024/02/07 22:51:08 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header.h"

static	void	add_mode_op(t_var *var, int *start, int *i, char *token_string)
{
    if (var->tokens == NULL)
		ft_lstadd_back_node(&var->tokens, ft_lstnew_node());
	token_string = ft_substr(var->inputline, *start, (*i) - (*start));
	*start = *i;
	if (*token_string)
		ft_lstadd_back_subnode(&var->tokens->redir, ft_lstnew_subnode(token_string)); //libero token_string dentro de lstnew_subnode.
}


static	void	add_cmd_or_whredir_or_param(t_var *var, char	**prev_token_string, int *whredir)
{
	if ((*prev_token_string) != NULL
		&& *(*prev_token_string) != '\0' && *(*prev_token_string) != '|' && var->tokens)
	{
		if (*whredir)
		{
			ft_lstadd_back_subnode(&var->tokens->where_redir, ft_lstnew_subnode((*prev_token_string)));
			*whredir = 0;
		}
		else if (var->tokens->token == NULL)
			ft_lstadd_back_subnode(&var->tokens->token, ft_lstnew_subnode((*prev_token_string)));
		else
			ft_lstadd_back_subnode(&var->tokens->params, ft_lstnew_subnode((*prev_token_string)));
	}
}

static	int	check_input_and_skip_spaces(t_var *var, int *start, int *i)
{
	if ((var->inputline == NULL) && *start < var->len_inputline)
		return (1);
	while (var->inputline[*start] == ' ' || var->inputline[*start] == '\t')
		(*start)++;
	*i = *start;
	return (0);
}

int	gnt_startpoint(t_var *var, int start)
{
	char		*token_string;
	int			i;
	int			wh_redir_flg;

	i = start;
	wh_redir_flg = 0;
	token_string = NULL;
	if (check_input_and_skip_spaces(var, &start, &i))
		return (0); // or 1 ? if *start < var->len_inputline
	while (i < var->len_inputline && !is_space_or_eof(var->inputline[i]))//until a space or EOF
	{
		if (isdouble_operator(var->inputline, i))// tokenize >> <<
		{
			if (token_string != NULL) // un operador marca el fin del token antieror (si hay). (ex: ls>test) '>' marca el fin del token 'ls'
				break ;
			i = i + 2;
			add_mode_op(var, &start, &i, token_string);
			wh_redir_flg = 1;
		}
		else if (isingle_operator(var->inputline, i)) // tokenize > < 
		{
			if (token_string != NULL)
				break ;
			i++;
			add_mode_op(var, &start, &i, token_string);
			wh_redir_flg = 1;
		}
		else
			token_string = check_word_rec(var, &start, &i, token_string);
	}
	add_cmd_or_whredir_or_param(var, &token_string, &wh_redir_flg);
	return (i);
}
