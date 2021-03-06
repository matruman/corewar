/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjamie <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/03 16:27:19 by sjamie            #+#    #+#             */
/*   Updated: 2020/03/15 16:36:10 by matruman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/asm.h"

static	void	common(t_main *main)
{
	parser(main);
	calc(main);
	coder(main);
	if (!main->a_flag)
		return (write_to_file(main));
	ft_printf("Dumping annotated program on standard output\n\
Program size : %d bytes\nName : \"%s\"\nComment : \"%s\"\n\n",
	main->last_token->pos + main->last_token->len,
	main->name, main->comment);
	print_code_info(main, 0, 0, 0);
}

static	void	setter(t_main *main, int ac, char *av[], int res)
{
	if (ac > 2)
		main->a_flag = 1;
	else
		main->filename = get_filename(av[res]);
}

static	void	help(void)
{
	die("Usage: ./asm [-a] <your_sourcefile.s> OR <your_binaryfile.cor>");
}

int				main(int ac, char *av[])
{
	t_main	*main;
	int		res;
	int		ch;

	if (is_disassembly(ac, av))
	{
		disassembly(av[1]);
		exit(0);
		return (0);
	}
	if (ac < 2 || ac > 3)
		help();
	res = check_args(ac, av);
	if (!res)
		help();
	if ((ch = open(av[res], O_RDONLY)) == -1)
		die(ft_strjoin("Can't read source file ", av[res]));
	main = init(ch);
	setter(main, ac, av, res);
	if (close(ch) == -1)
		die(ft_strjoin("Can't close source file ", av[res]));
	common(main);
	exit(0);
	return (0);
}
