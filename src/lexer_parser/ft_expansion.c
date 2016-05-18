/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expansion.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsanzey <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/28 15:18:06 by tsanzey           #+#    #+#             */
/*   Updated: 2016/03/28 15:18:39 by tsanzey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "shell.h"

t_token		*join_till_space(t_token *tok)
{
	char	*str;
	t_token	*tmp;

	tmp = tok;
	tok = tok->next;
	while (tok && tok->type == WORDS)
	{
		str = tmp->content;
		tmp->content = ft_strjoin(tmp->content, tok->content);
		free(str);
		pop_middle_token(tok);
		tok = tok->next;
	}
	return (tmp);
}

t_token		*ft_tild_expand(t_token *tok)
{
	t_token	*tmp;
	int		i;
	char	*str;

	tmp = tok;
	while (tmp)
	{
		if (tmp->type == TILD)
		{
			tmp->type = WORDS;
			if ((i = sh_is_new_var("HOME")) >= 0)
			{
				str = tmp->content;
				tmp->content = ft_strdup(&(ft_strchr(g_env[i], '=')[1]));
				free(str);
				if (check_next_token(tmp) == WORDS)
					tmp = join_till_space(tmp);
			}
		}
		tmp = tmp->next;
	}
	return (tok);
}
