/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hist_list_ft.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbaldy <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/29 18:41:27 by cbaldy            #+#    #+#             */
/*   Updated: 2016/05/02 11:54:58 by dbaldy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static t_hist_list	*create_new_hist(t_hist_list *a, t_hist_list *buf)
{
	t_hist_list		*new;

	if ((new = (t_hist_list*)malloc(sizeof(t_hist_list))) == NULL)
		return (NULL);
	new->old = line_list_dup(buf->old);
	new->next = NULL;
	new->nb = buf->nb;
	if (a != NULL)
		a->next = new;
	new->previous = a;
	return (new);
}

t_hist_list			*copy_hist(t_hist_list *hist)
{
	t_hist_list		*buf;
	t_hist_list		*a;

	if (hist == NULL)
		return (NULL);
	a = NULL;
	buf = hist;
	while (buf)
	{
		a = create_new_hist(a, buf);
		buf = buf->next;
	}
	while (a->previous)
		a = a->previous;
	return (a);
}

int					clear_hist(t_hist_list **hist)
{
	t_hist_list		*buf;
	t_hist_list		*a;

	if (*hist == NULL)
		return (0);
	while ((*hist)->previous)
		*hist = (*hist)->previous;
	buf = *hist;
	a = buf->next;
	while (buf)
	{
		buf->next = NULL;
		buf->previous = NULL;
		line_list_free(buf->old);
		free(buf);
		buf = a;
		if (a != NULL)
			a = a->next;
	}
	(*hist) = NULL;
	return (0);
}

int					hist_control_size(t_hist_list **hist)
{
	int			i;
	t_hist_list *tmp[2];

	i = 0;
	tmp[0] = *hist;
	while (tmp[0] != NULL)
	{
		i++;
		tmp[1] = tmp[0];
		tmp[0] = tmp[0]->next;
	}
	if (i > HIST_SIZE)
	{
		tmp[0] = tmp[1]->previous;
		tmp[0]->next = NULL;
		line_list_free(tmp[1]->old);
		free(tmp[1]);
	}
	return (0);
}
