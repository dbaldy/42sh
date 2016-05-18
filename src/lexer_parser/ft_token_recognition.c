/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_token_recognition.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsanzey <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/28 15:12:24 by tsanzey           #+#    #+#             */
/*   Updated: 2016/04/30 15:52:20 by cbaldy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "parse_build_list.h"

t_token		*pop_middle_token(t_token *tok)
{
	t_token *tmp;

	if (tok->prev)
		tok->prev->next = tok->next;
	if (tok->next)
		tok->next->prev = tok->prev;
	tmp = tok->next;
	free(tok->content);
	free(tok);
	tok = NULL;
	if (tmp == NULL)
		return (NULL);
	return (tmp);
}

t_token		*join_quoted(t_token *tok, t_sym sym)
{
	t_token	*tmp;
	t_token	*tmp1;
	char	*str;

	tmp = tok;
	while (tmp)
	{
		if (tmp->type == sym && tmp->next && tmp->next->type != sym)
		{
			tmp1 = tmp->next;
			tmp = tmp1->next;
			while (tmp && tmp->type != sym)
			{
				str = tmp1->content;
				tmp1->content = ft_strjoin(tmp1->content, tmp->content);
				tmp = pop_middle_token(tmp);
				free(str);
			}
		}
		if (tmp)
			tmp = tmp->next;
	}
	return (tok);
}

void		return_type_quoted(t_token *tok)
{
	int		is_quoted;
	t_sym	sym;

	is_quoted = 0;
	sym = -3;
	while (tok)
	{
		if (is_quoted && tok->type == BACKSLASH && isinibed(tok))
		{
			tok->next->type = WORDS;
			tok = pop_middle_token(tok);
		}
		if (is_quoted && tok->type != sym)
			tok->type = WORDS;
		else if (is_quoted && tok->type == sym)
			is_quoted = 0;
		else if (is_quoted == 0 &&
			(tok->type == QUOTES || tok->type == SINGLE_QUOTES))
		{
			sym = tok->type;
			is_quoted = 1;
		}
		tok = tok->next;
	}
}

t_token		*check_minus(t_token *tok)
{
	t_token	*tmp;
	char	*tmp1;

	tmp = tok;
	if (!tok)
		return (NULL);
	while (tmp->next)
	{
		if (tmp->type == MINUS && check_next_token(tmp) == WORDS)
		{
			tmp1 = tmp->next->content;
			tmp->next->content = ft_strjoin(tmp->content, tmp->next->content);
			tmp->type = WORDS;
			free(tmp1);
			if (tmp == tok)
				tok = tok->next;
			tmp = pop_middle_token(tmp);
		}
		else
			tmp = tmp->next;
	}
	return (tok);
}

t_parse		*ft_checking_syntax(t_token *tok)
{
	tok = inibitor_handler(tok);
	return_type_quoted(tok);
	tok = join_quoted(tok, QUOTES);
	tok = join_quoted(tok, SINGLE_QUOTES);
	tok = ft_subshell(tok);
	tok = check_dollar(tok);
	tok = ft_tild_expand(tok);
	ft_edit_useless(tok);
	if ((tok = ft_token_removal(tok, QUOTES)) == NULL)
		return (NULL);
	if ((tok = ft_token_removal(tok, SINGLE_QUOTES)) == NULL)
		return (NULL);
	tok = join_tokens(tok);
	if ((tok = ft_token_removal(tok, WHITESPACE)) == NULL)
		return (NULL);
	if (!tok)
		return (NULL);
	if (!(ft_command_isvalid(tok)))
	{
		free_token_list(tok);
		return (NULL);
	}
	return (parse_build_list(tok));
}
