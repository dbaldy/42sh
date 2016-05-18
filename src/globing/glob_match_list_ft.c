/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glob_match_list_ft.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaldy <dbaldy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/31 18:54:50 by dbaldy            #+#    #+#             */
/*   Updated: 2016/04/26 17:38:42 by dbaldy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "glob.h"

int				clear_matchlist(t_glob_list *match_list)
{
	t_glob_list	*tmp;

	if (match_list == NULL)
		return (0);
	while (match_list->prev)
		match_list = match_list->prev;
	tmp = match_list->next;
	free(match_list->var);
	if (match_list->next)
		tmp->prev = NULL;
	match_list->next = NULL;
	match_list->prev = NULL;
	free(match_list);
	if (tmp != NULL)
		clear_matchlist(tmp);
	return (0);
}

t_glob_list		*add_elem_glob(char *match, t_glob_list *glob)
{
	t_glob_list	*new;

	if ((new = (t_glob_list*)malloc(sizeof(t_glob_list))) == NULL)
		return (NULL);
	new->var = ft_strdup(match);
	new->next = NULL;
	new->prev = glob;
	if (glob != NULL)
		glob->next = new;
	return (new);
}
