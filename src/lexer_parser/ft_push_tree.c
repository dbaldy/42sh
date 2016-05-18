/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_push_tree.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsanzey <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/28 15:16:15 by tsanzey           #+#    #+#             */
/*   Updated: 2016/03/28 15:16:51 by tsanzey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

t_parse	*ft_push_output(t_parse *head, t_token *tok)
{
	t_parse	*new;
	t_parse	*tmp;

	if (!(new = (t_parse*)malloc(sizeof(t_parse))))
		return (NULL);
	if (!(new->arg = (char **)malloc(sizeof(char *) * 3)))
		return (NULL);
	if (tok->prev->type == NUMBERS)
		new->arg[0] = ft_strdup(tok->prev->content);
	else
		new->arg[0] = ft_strdup("1");
	new->arg[1] = ft_strdup(tok->next->content);
	new->arg[2] = NULL;
	new->type = tok->type == DIPLE_R ? GREAT : D_GREAT;
	new->next = NULL;
	if (!head)
		head = new;
	else
	{
		tmp = head;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
	return (head);
}

t_parse	*ft_push_input(t_parse *head, t_token *tok)
{
	t_parse	*new;
	t_parse	*tmp;

	if (!(new = (t_parse*)malloc(sizeof(t_parse))))
		return (NULL);
	if (!(new->arg = (char **)malloc(sizeof(char *) * 3)))
		return (NULL);
	new->arg[0] = ft_strdup(tok->next->content);
	if (tok->prev->type == NUMBERS)
		new->arg[1] = ft_strdup(tok->prev->content);
	else
		new->arg[1] = ft_strdup("0");
	new->arg[2] = NULL;
	new->type = tok->type == DIPLE_L ? LESS : D_LESS;
	new->next = NULL;
	if (!head)
		head = new;
	else
	{
		tmp = head;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
	return (head);
}
