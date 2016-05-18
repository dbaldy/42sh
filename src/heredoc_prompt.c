/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_prompt.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbaldy <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/02 12:28:57 by cbaldy            #+#    #+#             */
/*   Updated: 2016/05/06 11:12:54 by cbaldy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static int	heredoc_read_prompt(t_line_list **first, char *eof)
{
	int		len;
	int		i;
	char	buf[10];

	len = read(STDIN_FILENO, buf, 10);
	ft_bzero(&buf[len], 10 - len);
	i = 0;
	if ((len != 1 || buf[0] < 31 || buf[0] > 127) &&
			(len != 1 || buf[0] != 10))
		i = heredoc_edit_line(buf, len, first);
	else if (len == 1)
		i = heredoc_write_line(first, buf[0], eof);
	return (i);
}

static int	heredoc_remove_last(t_line_list **first)
{
	t_line_list	*tmp;

	tmp = NULL;
	if ((*first)->previous != NULL)
	{
		tmp = (*first)->previous;
		tmp->next = NULL;
	}
	(*first)->previous = NULL;
	com_list_free((*first)->begin);
	free(*first);
	*first = tmp;
	return (0);
}

static char	*heredoc_retrieve(t_line_list **first, int i)
{
	t_line_list	*tmp;
	char		*str;

	tmp = NULL;
	if (i == 4 || i == 3)
		ft_putchar_fd('\n', STDIN_FILENO);
	if (i == 3)
	{
		line_list_free(*first);
		return (NULL);
	}
	heredoc_remove_last(first);
	if (*first == NULL)
		return (NULL);
	while (*first != NULL && (*first)->previous != NULL)
		*first = (*first)->previous;
	str = line_list_retrieve(*first);
	line_list_free(*first);
	return (str);
}

static char	*heredoc_prompt(char *eof)
{
	int			i;
	t_line_list	*first;

	sh_set_term();
	ft_tputs("cd", 1, 0);
	ft_dprintf(STDOUT_FILENO, "heredoc> ");
	g_local->curs = 11;
	if ((first = line_list_new(10)) == NULL)
		return (NULL);
	i = 0;
	while (i != 10 && i != 3 && i != 4)
		i = heredoc_read_prompt(&first, eof);
	sh_reset_term();
	return (heredoc_retrieve(&first, i));
}

int			heredoc_main(t_tree *root)
{
	char	*str;
	int		*save;

	save = fd_save();
	sh_reset_std_fd();
	str = heredoc_prompt(root->cmd[0]);
	fd_reset(save);
	free(save);
	if (root->cmd[1] != NULL)
		free(root->cmd[1]);
	if (str != NULL)
		root->cmd[1] = str;
	else
		root->cmd[1] = NULL;
	return (0);
}
