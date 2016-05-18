/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_build_cmd.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbaldy <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/22 10:22:10 by cbaldy            #+#    #+#             */
/*   Updated: 2016/05/09 12:57:18 by cbaldy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tree_build_cmd.h"
#include "shell.h"

static int	tree_place_sshell(t_tree *new, t_tree **root)
{
	t_tree *leg;

	if ((leg = sh_lexer_parser(new->cmd[0])) == NULL)
		return (-1);
	new->right = leg;
	ft_free_tab(new->cmd);
	new->cmd = NULL;
	if (*root == NULL)
	{
		*root = new;
		return (0);
	}
	return (tree_place_cmd(new, root));
}

static int	tree_place_type_and(t_tree *new, t_tree **root)
{
	t_tree	*tmp;

	tmp = *root;
	if (tmp->types == END)
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

static int	tree_insert_elem(t_tree *new, t_tree **root)
{
	if (new->types == S_SHELL)
		return (tree_place_sshell(new, root));
	else if (*root == NULL)
	{
		*root = new;
		return (0);
	}
	else if (new->types == CMD)
		return (tree_place_cmd(new, root));
	else if (new->types < TUBES)
		return (tree_place_type_red(new, root));
	else if (new->types < AND_IF)
		return (tree_place_type_pipe(new, root));
	else if (new->types < END)
		return (tree_place_type_and(new, root));
	else
		return (tree_place_type_end(new, root));
}

t_tree		*tree_build_cmd(t_parse *head)
{
	t_tree	*root;
	t_tree	*new;
	t_parse	*tmp;

	root = NULL;
	while (head != NULL)
	{
		if ((new = tree_new_elem(head->arg, head->type)) == NULL)
			return (NULL);
		if (tree_insert_elem(new, &root) < 0)
		{
			if (new != NULL)
				exec_free_root(new);
			exec_free_root(root);
			parse_list_free(head->next);
			free(head);
			root = NULL;
			break ;
		}
		tmp = head->next;
		free(head);
		head = tmp;
	}
	return (root);
}
