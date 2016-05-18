/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glob_new_string.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaldy <dbaldy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/31 17:45:18 by dbaldy            #+#    #+#             */
/*   Updated: 2016/05/09 11:18:37 by dbaldy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "glob.h"

static char			*word_to_glob(char *str, int i)
{
	int		j;
	char	*tmp;

	j = i;
	while (i > 1 && ((str[i - 1] != ' ' && str[i - 1] != '\n') ||
	((str[i - 1] == ' ' || str[i - 1] == '\n') && i > 1 &&
		str[i - 2] == 0x5c)))
		i--;
	while (str[j] && ((str[j] != ' ' && str[j] != '\n') || (j > 0 &&
	(str[j] == ' ' || str[j] == '\n') && str[j - 1] == 0x5c)))
		j++;
	tmp = ft_strsub(str, i, j - i);
	return (tmp);
}

static int			glob_maison(char **str, int *j)
{
	t_glob_list	*match_list;
	char		*word;
	int			i;

	i = *j;
	if ((*str)[i] == '[' && ft_strchr(&((*str)[i + 1]), ']') == NULL)
		return (-1);
	word = word_to_glob(*str, i);
	match_list = glob_progressiv(word);
	if (match_list == NULL)
	{
		free(word);
		return (-1);
	}
	glob_modif_str(str, match_list, word, j);
	clear_matchlist(match_list);
	free(word);
	return (0);
}

static int			glob_new_string(char **str)
{
	int			i;

	i = 0;
	replace_dollars(str);
	while ((*str)[i])
	{
		if ((*str)[i] == ' ' && (*str)[i + 1] == '~' && ((*str)[i + 2] == '/'
				|| (*str)[i + 2] == ' ' || (*str)[i + 2] == '\n'))
			alias_ihome(str, i + 1);
		else if ((*str)[i] == 0x22 || (*str)[i] == 0x27)
			escape_quotes(*str, &i, (*str)[i]);
		else if ((*str)[i] == '$' && (*str)[i + 1] == '?')
			i += 2;
		else if ((*str)[i] == '*' || (*str)[i] == '?' || (*str)[i] == '[')
		{
			if (glob_maison(str, &i) < 0)
				return (-1);
		}
		else if ((*str)[i] != '\0')
			i = ((*str)[i] == 0x5c) ? i + 2 : i + 1;
	}
	return (0);
}

static int			parse_bckquotes(char **glob)
{
	int		i;
	char	*buf;

	i = 0;
	while ((*glob)[i])
	{
		if (((*glob)[i] == ')' || (*glob)[i] == '`') && (i == 0 ||
				(i > 1 && (*glob)[i - 1] != 0x5c)))
		{
			buf = ft_ireplace_str(*glob, " ", i, 0);
			free(*glob);
			*glob = buf;
			i += 2;
		}
		i++;
	}
	return (0);
}

int					glob_it(char **glob, char *str)
{
	parse_bckquotes(glob);
	if (glob_new_string(glob) == -1)
	{
		free(*glob);
		ft_dprintf(STDERR_FILENO, "sh: no matches found: %s\n", str);
		return (1);
	}
	if (ft_strlen(*glob) > 16384)
	{
		free(*glob);
		ft_dprintf(STDERR_FILENO, "sh: argument list too long: %s\n", str);
		return (1);
	}
	return (0);
}
