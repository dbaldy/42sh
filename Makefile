# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cbaldy <marvin@42.fr>                      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2015/12/14 14:42:25 by cbaldy            #+#    #+#              #
#    Updated: 2016/05/08 17:28:57 by cbaldy           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = 42sh

SRC = main.c sh_set_term.c sh_read_prompt.c sh_command.c sh_builtin_cd.c \
	  sh_builtin_env.c ft_1_aux.c sh_builtin_exit.c cd_exist_error.c \
	  sh_builtin_setenv.c sh_add_var_env.c sh_builtin_unsetenv.c \
	  cd_get_path.c sh_set_env.c env_custom.c cd_substitution.c yank_line.c\
	  sh_interpret_tree.c ft_tputs.c term_edit_line.c com_list_ft.c \
	  term_write_line.c hist_list_ft.c print_command.c copy_end_mode.c \
	  com_list_ft2.c term_finish_line.c exec_subshell.c\
	  term_mv_cursor.c sh_minishell.c get_next_line.c hist_change.c \
      exec_redout.c exec_redin.c exec_pipe_and_or.c sh_get_exec_path.c \
	  sh_builtin_echo.c hash_get_tab.c sh_builtin_hash.c sh_execute.c \
	  sh_retrieve_cmd_line.c cd_get_path2.c sh_builtin_history.c \
	  line_list_ft.c term_mv_horizontal.c hist_static.c cd_env_cdpath.c \
	  exec_heredoc.c heredoc_write_line.c heredoc_edit_line.c fd_save.c \
	  hashtable.c hashtable2.c exec_backquotes.c mini_finish_line.c\
	  line_list_ft2.c heredoc_prompt.c ed_ctrld_line.c

TAB = tab_mode.c tab_select.c tab_nb_col.c tab_nb_row.c tab_cursor_mvt.c \
	  tab_line_management.c tab_complete_line.c tab_prepare_select.c\
      tab_list_path.c tab_string_match.c tab_supp.c manage_search_hist.c\
	  search_history.c suppr_elem.c prepare_search_bar.c

GLOB = escape_quotes.c glob_build_list.c glob_match_list_ft.c glob_new_string.c\
	   glob_match.c glob_modif_str.c glob_progressiv.c glob_replace_dol.c \
	   glob_bang_str.c glob_replace_bang.c glob_bang_digit.c remove_spec_car.c

LEX = lexer.c ft_token_recognition.c ft_free.c \
	  ft_push_tree.c ft_misc.c pipe_grammar.c ft_great_grammar.c \
	  ft_input_correct.c tree_ft.c tree_build_cmd.c \
	  ft_isspace.c ft_catplus.c sh_lexer_parser.c ft_expansion.c \
	  ft_variable_expand.c sh_preparse.c grammar.c \
	  parse_build_list.c parse_list_ft.c parse_get_cmd.c parse_get_redir.c \
	  ft_token_edition.c ft_push_intab.c ft_inibitors.c sh_backquotes.c sh_subcmd.c\

INCLUDE = -I includes/

FLAG = -Wall -Werror -Wextra

LIBRARIES = -L libft/ -L ft_printf -lft -lftprintf -ltermcap

CGLOB = $(addprefix src/globing/, $(GLOB))
CFILES = $(addprefix src/, $(SRC))
CLEX = $(addprefix src/lexer_parser/, $(LEX))
CTAB = $(addprefix src/completion/, $(TAB))

OBJ = $(SRC:.c=.o)
OBJ2 = $(LEX:.c=.o)
OBJ3 = $(TAB:.c=.o)
OBJ4 = $(GLOB:.c=.o)

all: $(NAME)

$(NAME):
	@gcc -c $(FLAG) $(INCLUDE) $(CFILES) $(CLEX) $(CTAB) $(CGLOB)
	@make -C libft/ fclean && make -C libft && make -C libft/ clean
	@make -C ft_printf/ fclean && make -C ft_printf && make -C ft_printf/ clean
	@gcc -o $(NAME) $(LIBRARIES) $(OBJ) $(OBJ2) $(OBJ3) $(OBJ4)
	@mkdir obj && mv $(OBJ) $(OBJ2) $(OBJ3) $(OBJ4) obj
	@echo "\x1b[32;02m$(NAME) ready\x1b[0;m"

clean:
	@/bin/rm -rf obj
	@/bin/rm -rf $(OBJ) $(OBJ2) $(OBJ3) $(OBJ4)
	@echo "\x1b[0;01mclean\x1b[0;m"

fclean: clean
	@/bin/rm -f $(NAME)
	@make -C libft/ fclean
	@make -C ft_printf/ fclean
	@echo "\x1b[0;01mfclean\x1b[0;m"

re: fclean all
