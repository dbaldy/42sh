/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_ft.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbaldy <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/22 11:37:07 by cbaldy            #+#    #+#             */
/*   Updated: 2016/05/09 12:55:58 by cbaldy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tree_build_cmd.h"
#include "shell.h"

t_tree		*tree_new_elem(char **cmd, int type)
{
	t_tree	*new;

	if ((new = (t_tree *)malloc(sizeof(t_tree))) == NULL)
		return (NULL);
	new->cmd = cmd;
	new->types = type;
	new->left = NULL;
	new->right = NULL;
	return (new);
}

int			tree_place_type_pipe(t_tree *new, t_tree **root)
{
	t_tree	*tmp;

	tmp = *root;
	if (tmp->types > TUBES)
	{
		tmp = tmp->right;
		(*root)->right = new;
		new->left = tmp;
	}
	else
	{
		tmp = *root;
		*root = new;
		new->left = tmp;
	}
	return (0);
}

int			tree_place_type_red(t_tree *new, t_tree **root)
{
	t_tree	*tmp;
	t_tree	*tmp2;

	if (new->types == D_LESS && g_local->le_mode == 0)
		heredoc_main(new);
	if ((*root)->right == NULL)
	{
		tmp = *root;
		*root = new;
		new->right = tmp;
	}
	else
	{
		tmp = *root;
		while (tmp->right != NULL && tmp->right->types > S_SHELL)
			tmp = tmp->right;
		tmp2 = tmp->right;
		tmp->right = new;
		new->right = tmp2;
	}
	return (0);
}

int			tree_place_cmd(t_tree *new, t_tree **root)
{
	t_tree	*tmp;

	tmp = *root;
	while (tmp->right != NULL)
		tmp = tmp->right;
	if (tmp->types < LESS)
	{
		ft_putendl("shell: double command error");
		return (-1);
	}
	else
		tmp->right = new;
	return (0);
}

int			tree_place_type_end(t_tree *new, t_tree **root)
{
	t_tree	*tmp;

	tmp = *root;
	*root = new;
	new->left = tmp;
	return (0);
}
