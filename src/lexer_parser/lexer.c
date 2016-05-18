/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsanzey <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/03 11:05:40 by tsanzey           #+#    #+#             */
/*   Updated: 2016/04/27 10:40:33 by cbaldy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "ft_printf.h"

int		ft_other_redirs(char *s, int i, int type)
{
	static const char	*token_types[] = {"\"", "\'", ">", ">>", "<",
	"<<", "|", ";", "&", "~", "\\", "$", "-", "(", ")", "\0"};

	if (s[i + 1] && type == DIPLE_R && s[i + 1] == s[i])
		return (DOUBLE_R);
	if (s[i + 1] && type == DIPLE_L && s[i + 1] == s[i])
		return (DOUBLE_L);
	if (s[i + 1] && type == DIPLE_R && s[i + 1] == *token_types[AMPERSAND])
		return (GREAT_AND);
	if (s[i + 1] && type == DIPLE_L && s[i + 1] == *token_types[AMPERSAND])
		return (LESS_AND);
	if (s[i + 1] && type == AMPERSAND && s[i + 1] == *token_types[DIPLE_R])
		return (AND_GREAT);
	if (s[i + 1] && type == AMPERSAND && s[i + 1] == *token_types[AMPERSAND])
		return (D_SAND);
	if (s[i + 1] && type == PIPE && s[i + 1] == *token_types[PIPE])
		return (D_PIPE);
	return (type);
}

int		ft_token_type(char *s, int i)
{
	int					type;
	int					j;
	static const char	*token_types[] = {"\"", "\'", ">", ">>", "<",
	"<<", "|", ";", "&", "~", "\\", "$", "-", "(", ")", "\0"};

	type = WORDS;
	j = 0;
	if (is_word_or(s, i))
		return (NUMBERS);
	if (ft_isspace(s[i]))
		return (WHITESPACE);
	while (*token_types[j])
	{
		if (s[i] == *token_types[j])
		{
			type = j;
			if (j == MINUS && s[i - 1] != '&')
				return (-1);
			if (j == DIPLE_R || j == DIPLE_L || j == AMPERSAND || j == PIPE)
				return (ft_other_redirs(s, i, type));
			return (type);
		}
		j++;
	}
	return (type);
}

char	*tok_content(char *s, int start, int type)
{
	int					i;
	char				*dst;
	static const char	*token_types[] = {"\"", "\'", ">", ">>", "<",
	"<<", "|", ";", "&", "~", "\\", "$", "-", "(", ")", "<&", ">&", "&>", "||",
	"&&", "\0"};

	if (type == DOUBLE_R || type == DOUBLE_L ||
		(type >= LESS_AND && type <= D_SAND))
	{
		dst = ft_strdup(token_types[type]);
		return (dst);
	}
	i = 0;
	if (type == WORDS)
	{
		while (s[start + i] && ft_token_type(s, start + i) == WORDS)
			i++;
	}
	else if (type == NUMBERS)
		while (s[start + i] && ft_token_type(s, start + i) == NUMBERS)
			i++;
	i = i == 0 ? 1 : i;
	dst = ft_strsub(s, start, i);
	return (dst);
}

int		ft_get_i(t_token *new, char *s, int i)
{
	if (new->type == DOUBLE_R || new->type == DOUBLE_L ||
		(new->type >= LESS_AND && new->type <= D_SAND))
		i++;
	if (ft_token_type(s, i) == WORDS)
		i = ft_next_token(s, i, WORDS);
	else if (ft_token_type(s, i) == NUMBERS)
		i = ft_next_token(s, i, NUMBERS);
	else
		i++;
	return (i);
}

t_token	*ft_tokeniser(char *s, t_token *head)
{
	t_token	*new;
	int		i;
	char	*str;

	i = 0;
	str = s;
	while (s[i])
	{
		if (s[i] == '`' && s[i - 1] != '\\' && find_closing(s, i))
			s = ft_backquotes(s, i);
		if (!s[i])
			break ;
		new = ft_allocate(new);
		new->type = ft_token_type(s, i);
		new->content = tok_content(s, i, new->type);
		head = ft_push_token(head, new);
		i = ft_get_i(new, s, i);
	}
	if (s != str)
		free(s);
	return (head);
}
