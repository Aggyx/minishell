/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 19:35:31 by smagniny          #+#    #+#             */
/*   Updated: 2024/01/22 17:10:49 by math             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADER_H
# define HEADER_H

# include <stdlib.h>
# include <fcntl.h>
# include <stdio.h>
# include <errno.h>
# include "libft/libft.h"
# include <string.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>


# include <stddef.h> // Incluye la librería para definir NULL
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/stat.h>


# define SINGLEQ '\''
# define DOUBLEQ '\"'

enum	e_main_errors {
	OPEN_FAIL = 1000,
	CLOSE_FAIL = 2000,
	PIPE_FAIL = 3000,
	FORK_FAIL = 4000,
	MALLOC_FAIL = 5000
};

//		·· subnode for storing multiple word instead of **array (USED IN T_NODE) ··
typedef struct s_subnode
{
	char				*content;
	struct s_subnode	*next;
}	t_subnode;

//		·· node w content of all combination of execution inside pipes ··
typedef struct s_node
{
	t_subnode			*token;
	t_subnode			*flags;
	t_subnode			*params;
	t_subnode			*redir;
	t_subnode			*where_redir;
	struct s_node		*next;
}	t_node;

//		·· node w content of a line of list environment variables ··
typedef struct s_node_env
{
	char				*line_env;
	int					exported;
	struct s_node_env	*next;
}	t_env;


//		·· All important variables passed by reference ··
typedef struct s_var
{
	size_t		std_in;
	size_t		std_out;
	size_t		fd_in;
	size_t		fd_out;
	t_env		*envp;
	char		*inputline;
	int			len_inputline;
	int			nb_node;
	int			exit_status;
	t_node		*tokens;
}	t_var;

// <--------------------------------------------------------------->

//						###### lexer/Parser ######
void		lexer(t_var *var);
int			gnt_startpoint(t_var *var, int start);
char		*check_word_rec(t_var *var, int *start, int *i, char *token_string);
int			is_space_or_eof(int c);
int			isingle_operator(char *line, int i);
int			isdouble_operator(char *line, int i);
int			issinglequote(int c);
int			isdoublequote(int c);
char		*ft_strjoinfreee(char *s1, char *s2);
char		*get_str_doublequoted(t_var *var, int *i, int *start);
char		*get_str_singlequoted(t_var *var, int *i, int *start);
char		*get_word(t_var *var, int *i, int *start);

// <--------------------------------------------------------------->

// 						###### get input ######
char		*get_inputline(t_var *var);

// <--------------------------------------------------------------->

// 						###### expansion ######
void		expansor(t_var *var, char **string, int doublequoted);
char		*expand(t_env *env, char *dollar_str);
void		*my_realloc(void *ptr, size_t originalLength, size_t newLength);
void		are_any_expansion(char **res, char **string, int ref);

// <--------------------------------------------------------------->

// 					######list node functions ######
void		ft_lstclear_node(t_node **lst);
void		ft_freenode(t_node **node);
t_node		*ft_lstnew_node(void);
t_node		*ft_lstlast_node(t_node *lst);
void		ft_lstadd_back_node(t_node **lst, t_node *new);


// <--------------------------------------------------------------->

// 				######list subnode functions ######
void		ft_lstclear_subnode(t_subnode **lst);
t_subnode	*ft_lstnew_subnode(char *content);
t_subnode	*ft_lstlast_subnode(t_subnode *lst);
void		ft_lstadd_back_subnode(t_subnode **lst, t_subnode *new);

// <--------------------------------------------------------------->

//			###### list environment functions ######
void		ft_expansor(t_var *var);
void		cpy_env(t_env **ptr, const char **envp);
void		ft_freeenv(t_env **lst);
t_env		*new_node_env(const char *line_env, int exported);
void		ft_addback_node_env(t_env **lst, t_env *new);
char		**envlist_to_array(t_env *envlist);

// <--------------------------------------------------------------->

//			###### redir funcs ######
int			handle_outfileredirection(t_var *var);
int			handle_infileredirection(t_var *var);
void		base_redir(t_var *var);
int			redir(t_var *var);

// <--------------------------------------------------------------->

//			###### External command execution ######
int			ft_exec(t_var *var);

// <--------------------------------------------------------------->

//			###### Builtins ######
int			cd(t_node *tokens);
void		echo(t_node *tokens);
int			run_builtin(t_var *var);
int			pwd(t_var	*var);
int			env(t_var *var);
int			export(t_var *var);
void		unset(t_var *var);

// <--------------------------------------------------------------->

int    process_handler(t_var *var);

int close_pipe_read(int *fd, int *status);
int close_pipe_write(int *fd, int *status);
int close_pipe(int *fd, int *status);
int destroy_pipe(int *fd, int *status);

#endif