/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glob_replace_bang.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaldy <dbaldy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/27 17:16:36 by dbaldy            #+#    #+#             */
/*   Updated: 2016/04/30 16:52:54 by dbaldy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "glob.h"

static int			rcpy_begin_line(char **str, int *i)
{
	char	*begin;
	char	*tmp;

	begin = ft_strsub(*str, 0, *i);
	tmp = ft_ireplace_str(*str, begin, *i, 2);
	*i += ft_strlen(begin);
	free(begin);
	free(*str);
	*str = tmp;
	return (0);
}

static int			replace_bangs(char **str, int *i)
{
	t_hist_list	*hist;

	if ((*str)[*i + 1] == '#')
		return (rcpy_begin_line(str, i));
	if ((hist = retrieve_history(2, NULL)) == NULL)
		return (-1);
	if (ft_isdigit((*str)[*i + 1]) == 1 || (*str)[*i + 1] == '!')
		return (direct_digit(str, i, &hist));
	else if ((*str)[*i + 1] == '-')
	{
		if (ft_isdigit((*str)[*i + 2]) == 1)
			return (backward_digit(str, i, &hist));
		else
			return (-1);
	}
	else if ((*str)[*i + 1] == '?')
		return (strstr_cmd(str, i, &hist));
	else
		return (strcmp_cmd(str, i, &hist));
	return (0);
}

static int			to_bang_or_not(int expand, char *str)
{
	if (expand == 1)
	{
		ft_dprintf(STDOUT_FILENO, "%s\n", str);
		return (1);
	}
	return (0);
}

static int			replace_bang(char **str)
{
	int		i;
	int		expand;

	i = 0;
	expand = 0;
	while ((*str)[i])
	{
		if ((*str)[i] == 0x27)
			escape_quotes(*str, &i, (*str)[i]);
		else if ((*str)[i] == '!')
		{
			expand = 1;
			if (replace_bangs(str, &i) < 0)
			{
				ft_printf("sh: no such event: %s\n", *str);
				return (-1);
			}
		}
		else if ((*str)[i] != '\0')
			i = ((*str)[i] == 0x5c) ? i + 2 : i + 1;
	}
	return (to_bang_or_not(expand, *str));
}

int					get_bangged(char **str, t_line_list **first)
{
	int		ret;

	if ((ret = replace_bang(str)) == 1)
	{
		line_list_free(*first);
		*first = str_to_line(*str, NULL);
		return (1);
	}
	else if (ret == 0)
		return (0);
	else
	{
		free(*str);
		*str = NULL;
		return (-1);
	}
}
