/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safe_funcs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 10:29:19 by fabi              #+#    #+#             */
/*   Updated: 2024/11/14 10:50:51 by fabi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*safe_malloc(size_t size)
{
	void	*ret;

	ret = malloc(size);
	if (ret == NULL)
	{
		write(2, "Error: Malloc failed\n", 21);
	}
	return (ret);
}
