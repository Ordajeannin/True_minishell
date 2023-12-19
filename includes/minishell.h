/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkorsako <pkorsako@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 12:09:51 by ajeannin          #+#    #+#             */
/*   Updated: 2023/12/19 17:47:11 by pkorsako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H

# define MINISHELL_H

/* * * * * Includes * * * * */
# include <stdio.h>
# include <unistd.h>
# include <fcntl.h>
# include <limits.h>
# include <stdlib.h>
# include <errno.h>
# include <signal.h>
# include <sys/ioctl.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <dirent.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <string.h>
# include <ctype.h>
# include "libft.h"

# define ERROR_NOENV 	"env is not found"
# define ERROR			-1

# define TOKEN_CMD		1
# define TOKEN_OPTION	2
# define TOKEN_AND		3
# define TOKEN_PIPE		4
# define TOKEN_INFILE	5
# define TOKEN_OUTFILE	6
# define TOKEN_DELIM	7
# define TOKEN_APPEND	8
# define TOKEN_FPATH	9
# define TOKEN_RPATH	10
# define TOKEN_OR		11
# define TOKEN_S_QUOTES 22
# define TOKEN_D_QUOTES 23
# define TOKEN_INTERDOT 0
# define TOKEN_INTERDOT_D_QUOTES 230
# define TOKEN_TEMP_VAR	666
# define ABORT_MISSION	42

# define YES			1
# define NO				0


/* * * * * Variable Globale * * * * */
extern int	g_error;

/* * * * * Structures * * * * */
struct s_test
{
	int	var_c;
	int	var_d;
};

typedef struct s_args
{
	char			*str;
	int				token;
	struct s_args	*next;
}	t_args;

typedef struct s_shell
{
	char	*home;
	char	*pwd;
	char	*is_pwd;
	char	*is_oldpwd;
	char	*oldpwd;
	char	*path;
	char	*shlvl;
	char	**cmd_paths;
	char	**input;
	int		is_work;
	char	*input_bis;
	int		error;
	char	*secret_pwd;
	t_args  *env_list;
	t_args  *list;
}	t_shell;

typedef struct s_args_list
{
	t_args				*head;
	struct s_args_list	*next;
}	t_args_list;

typedef struct s_split
{
	int		i;
	int		j;
	int		start;
	int		flag;
	char	**result;
}	t_split;

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -//
/* * * * * Prototypes Fonctions Minishell * * * * */
int			is_minishell(t_shell *shell);
void		ft_gain_place(char **av, t_args **list, \
			t_args **env_list);
int			msg(char *msg);
int			handle_env(char **env, t_shell *envcpy);
void		init_shell(t_shell *shell);
int			parsing_input(char **input);
void		args_handle(t_args *list, t_shell *shell, t_args **env_list);
void		was_unclosed_quotes(t_args **list);
size_t		is_quotes(char *str, t_args **list, const char *input, int flag);
int			process_not_s_quotes(t_args *node, t_args **env_list);
void		is_there_a_redirection(t_args **list);
void		c_est_ma_direction(int token, t_args **list);
void		plus_de_nouvelle(const char *str);
int			is_correct_format(t_args **list);
void		free_sublists(t_args_list *stock);
int			update_args2(t_args **list, t_args **env_list);
int			handle_heredoc(t_args **input);
int			handle_multi_heredoc(t_args **stock);
void		signal_heredoc(int sig);


//Pipe
void		create_sublists(t_args *list, t_shell *shell, t_args **env_list);
char		*check_if_there_is_a_lost_pipe(char *input);
t_args_list	*stock_sublist(t_args **list);
void		print_sublists(t_args_list *stock);
void		ft2_lstadd_back(t_args_list **lst, t_args_list *new);
t_args_list	*ft2_lstlast(t_args_list *lst);
void		close_pipe(int pipe_fd[2]);
void		redirect_input(int prev_pipe_fd[2]);
void		redirect_output(int next_pipe_fd[2]);
void		execute_child1(int prev_pipe_fd[2], int next_pipe_fd[2]);
void		execute_child2(t_args_list *current, t_shell *shell, \
			t_args **env_list, char *input);
void		execute_parent(int prev_pipe_fd[2], int next_pipe_fd[2]);

//Tok
char		*ft_strtok(char *input, char **delim, t_shell *shell);
int			update_args(t_args **list);
int			tokenize_args(char *input, int flag);
size_t		delimit_to_token(char *s, t_args **list, const char **input);
char		*is_env_var(char *str, t_args **env_list);
char		**ft_split_arg(char *str);

//libc parsing
int			ft_strcmp(const char *s1, const char *s2);
char		*ft_strchr(const char *s, int c);
char		*ft_strpbrk(const char *s, const char *charset);
size_t		ft_strspn(const char *s, char **accept, t_shell *shell);
size_t		ft_strcspn(const char *s, char **reject, t_shell *shell);
char		*ft_strncpy(char *dst, const char *src, int n);
char		*ft_strrchr(const char *str, int ch);
int			is_alphanum(char c);
void		*ft_realloc(void *ptr, size_t old_size, size_t new_size);
char		*ft_strcat(char *str1, const char *str2);
char		*ft_strcpy(char *dest, const char *src);
char		*word_dup(char *str, int start, int finish);

//List concerns
int			from_input_to_list_of_args(char *input, t_shell *shell, \
	t_args **e_list);
int			loop_args(t_shell *shell, t_args **list);
void		clear_args_list(t_args **list);
void		add_arg(t_args **list, char *str, int token);

//Bultins
int			ft_echo(t_args *list, t_args **env_list, t_shell *shell);
int			ft_cd(t_args *list, t_shell *shell, t_args *env_list);
int			ft_pwd(t_shell *shell);
int			ft_env(t_args *list, t_args **env_list, t_shell *shell);
int			ft_unset(t_args *list, t_shell *shell, t_args *env_list);
int			ft_export(t_args *list, t_shell *shell, t_args *env_list);
int			ft_exit(t_args *list, t_args *env_list, \
	t_shell *shell);

//Other commands
int			all_cmd(t_args *arg, t_shell *shell, t_args **list, \
	t_args **env_list);
void		change_env_cd(t_args **env_list, char *new_str, \
	char *change_value);
int			cd_move_and_change(t_args *env_list, t_shell *shell);
int			change_env_exp(t_args **env_list, char *name_env, char *value);
int			update_last_ve(t_args *list, t_args **env_list);
int			parse_export(t_args *list);
int			searchin_env(t_args **env_list, t_args *list, t_shell *shell);
char		*is_path_or_cmd(char **paths, char *cmd, t_shell *shell, \
	t_args **env_list);
void		shell_change(t_shell *shell, char *str, char *value);
int			set_env(t_args **env_list, char **env, t_shell *shell);
void		add_env(t_args **env_list, char *str);
int			ft_plus_shell(t_shell *shell, t_args **env_list);
void		signal_handler(int sig);
int			export_out_args(t_args **env_list);
int			change_error(t_args **env_list, t_shell *shell, int value);
void		code_error(int code);
int			handle_error(int code_err);

int			set_error_nb(int error, int update);
t_args		*create_env(t_shell *data, char **envp);
t_args		*find_a(char *var, t_args *env);
void		update_pwd(t_args *env_list, t_shell *shell);
char		*ft_getcwd();
int			keep_tmp_fd(int tmp_fd, int update);

//Helpful function
char		*ft_strdupto_n(char *str, char c);
char		*ft_strdup_from(char *str, char c);
char		*ft_strjoin_free(char *s1, char *s2);
char		*from_end_to_char(char *str, char c);
int			is_only_equal(char *str, char c);
int			len_targs(t_args *list);
void		free_everything(t_shell *shell, t_args *list, t_args *env_list);

int			ft_strlen_double(char **str);
int			count_back(char	*str);
int			count_dir(t_shell *shell);
char		**dup_double_string(t_args **e_list);
int			is_numeric(char *str);
t_args		*copy_list(t_args* source);
char		**ft_sort(t_args **env_list);


//Print Things
void		shell_style(t_shell *shell);
void		print_args_list(t_args **list);
char		*get_username(t_args **env_list, t_shell *shell);
char		*get_pwd(void);
int			set_empty_env(t_shell *shell, t_args **env_list);
char		*prompt_cmd(t_shell *shell);

void		print_shell(t_shell *shell);


//Norme
char		*help_itp1(t_args *env_list, t_shell **shell, DIR **dir);
char		*help_itp2(DIR **dir);
int			help_cee(char **current_name, t_args **current);
int			help_cee2(char **current_name, t_args **current, char **result,
			char **name_env);
int			help_s_e(t_args **temp, t_args **current, t_args **env_list);
int			help_set_env(t_args **env_list, char **env, int *i,
			char **identifier);
int			help_set_env2(t_args **env_list, t_shell **shell, char **identifier,
			int *i);

int		handle_quotes(t_args **list, t_args **e_list);
int		help_fitloa(t_args **list, t_args **e_list);
t_args *create_arg(char *str, int token);
void	delete_null_nodes(t_args **list);
int		is_quote(char c, char *flag);

#endif
