/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mdr.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/12 16:05:00 by ldedier           #+#    #+#             */
/*   Updated: 2019/01/23 19:17:35 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>


int main(int argc, char **argv)
{
	(void)argc;
	(void)argv;

	write(1, "hehehe\n", 7);
	while(1);
	return (0);
}